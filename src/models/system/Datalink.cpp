
#include "mixr/models/system/Datalink.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/CommRadio.hpp"
#include "mixr/models/system/Radio.hpp"
#include "mixr/models/system/trackmanager/TrackManager.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/Message.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/util/system_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Datalink, "Datalink")

BEGIN_SLOTTABLE(Datalink)
   "radioId",           // 1: Radio ID (see note #1)
   "maxRange",          // 2: Max range of the datalink (w/o a radio model)
   "radioName",         // 3: Name of the (optional) communication radio mode
   "trackManagerName",  // 4: Track Manager Name
END_SLOTTABLE(Datalink)

BEGIN_SLOT_MAP(Datalink)
    ON_SLOT(1, setSlotRadioId,          base::Integer)
    ON_SLOT(2, setSlotMaxRange,         base::Length)
    ON_SLOT(3, setSlotRadioName,        base::String)
    ON_SLOT(4, setSlotTrackManagerName, base::String)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Datalink)
    ON_EVENT_OBJ(DATALINK_MESSAGE, onDatalinkMessageEvent, base::Object)
END_EVENT_HANDLER()

Datalink::Datalink()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void Datalink::initData()
{
   inQueue = new base::safe_queue<base::Object*>(MAX_MESSAGES);
   outQueue = new base::safe_queue<base::Object*>(MAX_MESSAGES);
}

void Datalink::copyData(const Datalink& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   noRadioMaxRange = org.noRadioMaxRange;
   radioId = org.radioId;
   useRadioIdFlg = org.useRadioIdFlg;

   sendLocal = org.sendLocal;
   queueForNetwork = org.queueForNetwork;

   {
      const base::String* p = nullptr;
      if (org.radioName != nullptr) {
         p = org.radioName->clone();
      }
      setRadioName( p );
      setRadio(nullptr);
   }

   {
      const base::String* p = nullptr;
      if (org.tmName != nullptr) {
         p = org.tmName->clone();
      }
      setTrackManagerName( p );
      setTrackManager(nullptr);
   }
}

void Datalink::deleteData()
{
   if (inQueue != nullptr && outQueue != nullptr) {
      clearQueues();
      delete inQueue;
      delete outQueue;
      inQueue = nullptr;
      outQueue = nullptr;
   }
   setRadio(nullptr);
   setRadioName(nullptr);
   setTrackManager(nullptr);
   setTrackManagerName(nullptr);
}

//------------------------------------------------------------------------------
// shutdownNotification() -- We're shutting down
//------------------------------------------------------------------------------
bool Datalink::shutdownNotification()
{
   clearQueues();
   setRadio(nullptr);
   setTrackManager(nullptr);
   setTrackManagerName(nullptr);

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned short Datalink::getRadioID() const
{
   unsigned short id{};
   if (useRadioIdFlg) {
      id = radioId;
   } else if (radio != nullptr) {
      id = radio->getRadioId();
   }
   return id;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// max datalink range
bool Datalink::setMaxRange(const double nm)
{
   noRadioMaxRange = nm;
   return true;
}

// Send to local players flag
bool Datalink::setLocalSendEnabled(const bool flg)
{
   sendLocal = flg;
   return true;
}

// Send to the network output queue
bool Datalink::setNetworkQueueEnabled(const bool flg)
{
   queueForNetwork = flg;
   return true;
}

// set our comm radio system
bool Datalink::setRadio(CommRadio* const p)
{
   if (radio != nullptr) {
      radio->setDatalink(nullptr);
      radio->unref();
   }
   radio = p;
   if (radio != nullptr) {
      radio->ref();
      radio->setDatalink(this);
   }
   return true;
}

//  Sets our radio's name
bool Datalink::setRadioName(const base::String* const p)
{
    if (radioName != nullptr) {
        radioName->unref();
    }
    radioName = p;
    if (radioName != nullptr) {
        radioName->ref();
    }
    return true;
}

// set the track manager
bool Datalink::setTrackManager(TrackManager* const tm)
{
    if (trackManager != nullptr) {
        trackManager->unref();
    }
    trackManager = tm;
    if (trackManager != nullptr) {
        trackManager->ref();
    }
    return true;
}

// set the track manager's name
bool Datalink::setTrackManagerName(const base::String* const name)
{
    if (tmName != nullptr) {
        tmName->unref();
    }
    tmName = name;
    if (tmName != nullptr) {
        tmName->ref();
    }
    return true;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Datalink::reset()
{
   clearQueues();
   // ---
   // Do we need to find the track manager?
   // ---
   if (getTrackManager() == nullptr && getTrackManagerName() != nullptr) {
        // We have a name of the track manager, but not the track manager itself
        const char* name{getTrackManagerName()->c_str()};
        // Get the named track manager from the onboard computer
        const auto ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
        if (ownship != nullptr) {
            OnboardComputer* obc{ownship->getOnboardComputer()};
            if (obc != nullptr) {
               setTrackManager(obc->getTrackManagerByName(name));
            }
        }
        if (getTrackManager() == nullptr) {
            // The assigned track manager was not found!
            //if (isMessageEnabled(MSG_ERROR)) {
            //std::cerr << "Datalink ERROR -- track manager, " << name << ", was not found!" << std::endl;
            //}
        }
   }
   // ---
   // Do we need to find the comm radio?
   // ---
   if (getRadio() == nullptr && getRadioName() != nullptr) {
        // We have a name of the radio, but not the radio itself
        const char* name{getRadioName()->c_str()};
        // Get the named radio from the component list of radios
        const auto ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
        if (ownship != nullptr) {
            const auto cr = dynamic_cast<CommRadio*>(ownship->getRadioByName(name));
            setRadio(cr);
        }
        CommRadio* rad{getRadio()};
        if (rad == nullptr) {
            // The assigned radio was not found!
            if (isMessageEnabled(MSG_ERROR)) {
                std::cerr << "Datalink ERROR -- radio, " << name << ", was not found!" << std::endl;
            }
        } else {
            rad->setDatalink(this);
            rad->setReceiverEnabledFlag(true);
            rad->setTransmitterEnableFlag(true);
        }
   }
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// dynamics() -- Age queues
//------------------------------------------------------------------------------
void Datalink::dynamics(const double)
{
    //age queues
    mixr::base::Object* tempInQueue[MAX_MESSAGES]{};
    int numIn{};
    Message* msg{};
    while ((numIn < MAX_MESSAGES) && inQueue->isNotEmpty()) {
        mixr::base::Object* tempObj{inQueue->get()};
        msg = dynamic_cast<Message*>(tempObj);
        if (msg != nullptr) {
            if (base::getComputerTime() - msg->getTimeStamp() > msg->getLifeSpan()) {
                //remove message by not adding to list to be put back into queue
                msg->unref();
            } else {
                tempInQueue[numIn++] = msg;
            }
        } else if (tempObj != nullptr) {
            tempInQueue[numIn++] = tempObj;
        }
    }
    if (numIn != 0) {
        for(int i = 0; i < numIn; i++) {
            inQueue->put(tempInQueue[i]);
        }
    }

    mixr::base::Object* tempOutQueue[MAX_MESSAGES]{};
    int numOut{};
    msg = nullptr;
    while((numOut < MAX_MESSAGES) && outQueue->isNotEmpty()) {
        mixr::base::Object* tempObj{outQueue->get()};
        msg = dynamic_cast<Message*>(tempObj);
        if(msg != nullptr) {
            if(base::getComputerTime() - msg->getTimeStamp() > msg->getLifeSpan()) {
                //remove message by not adding to list to be put back into queue
                msg->unref();
            } else {
                tempOutQueue[numOut++] = msg;
            }
        } else if (tempObj != nullptr) {
            tempOutQueue[numOut++] = tempObj;
        }
    }
    if (numOut != 0) {
        for(int i = 0; i < numOut; i++) {
            outQueue->put(tempOutQueue[i]);
        }
    }
}

//------------------------------------------------------------------------------
// sendMessage() -- send the datalink message out to the world.
//------------------------------------------------------------------------------
bool Datalink::sendMessage(base::Object* const msg)
{
   bool sent{};

   // If we can send to our local players directly (or via radio)
   if (sendLocal) {
      // ---
      // Have a comm radio -- then we'll just let our companion radio system handle this
      // ---
      if (radio != nullptr) {
         sent = radio->transmitDataMessage(msg);
      }

      // ---
      // No comm radio -- then we'll send this out to the other players ourself.
      // ---
      else if (getOwnship() != nullptr) {
         WorldModel* sim{getWorldModel()};
         if (sim != nullptr) {

            base::PairStream* players{sim->getPlayers()};
            if (players != nullptr) {

            base::List::Item* playerItem{players->getFirstItem()};
            while (playerItem != nullptr) {

               base::Pair* playerPair{static_cast<base::Pair*>(playerItem->getValue())};
               Player* player{static_cast<Player*>(playerPair->object())};

               if (player->isLocalPlayer()) {
                  // Send to active, local players only (and not to ourself)
                     if ((player->isActive() || player->isMode(Player::Mode::PRE_RELEASE)) && player != getOwnship() ) {
                     player->event(DATALINK_MESSAGE, msg);
                  }
                  playerItem = playerItem->getNext();
               } else {
                  // Networked players are at the end of the list,
                  // so we can stop now.
                  playerItem = nullptr;
               }

            }
               players->unref();
               players = nullptr;
            }
         }
         sent = true;
      }
   }

   // ---
   // and let any (optional) outgoing queue know about this.
   // ---
   if (queueForNetwork) {
      Player* ownship{getOwnship()};
      if (ownship != nullptr) {
         if (ownship->isLocalPlayer()) {
            queueOutgoingMessage(msg);
         }
      }
   }

   return sent;
}

//------------------------------------------------------------------------------
// receiveMessage() --
//------------------------------------------------------------------------------
base::Object* Datalink::receiveMessage()
{
   // Get the next one off of the incoming message queue.
   return inQueue->get();
}

//------------------------------------------------------------------------------
// queueIncomingMessage() -- Queue up an incoming message
//------------------------------------------------------------------------------
bool Datalink::queueIncomingMessage(base::Object* const msg)
{
   // Only queue message if Ownship is local.  Networked player messages are processed on their local systems
   if ((getOwnship() == nullptr) || !(getOwnship()->isLocalPlayer())) {
      return true;
   }

   //if (isMessageEnabled(MSG_INFO)) {
   //std::cout << getOwnship()->getID() << "\tincomming QQueue Size: " << inQueue->entries() << std::endl;
   //}

   if(inQueue->isFull()) {
      if (isMessageEnabled(MSG_WARNING)) {
         std::cerr << "dumping 10 oldest messages in Datalink::inQueue" << std::endl;
      }

      for(int i = 0; i < 10; i++) {
         base::Object* obj{inQueue->get()};
         obj->unref();
      } //clear out 10 oldest messages
   }
   if (msg != nullptr) {
      msg->ref();
      inQueue->put(msg);
   }
   return true;
}

//------------------------------------------------------------------------------
// queueOutgoingMessage() -- Queue up an out going message --
//------------------------------------------------------------------------------
bool Datalink::queueOutgoingMessage(base::Object* const msg)
{
    //if (isMessageEnabled(MSG_INFO)) {
    //std::cout << getOwnship()->getID() << "\tOutgoing QQueue Size: " << outQueue->entries() << std::endl;
    //}

    if (outQueue->isFull()) {
        if (isMessageEnabled(MSG_WARNING)) {
            std::cerr << "dumping 10 oldest messages in Datalink::outQueue" << std::endl;
        }

        for(int i = 0; i < 10; i++) {
            base::Object* obj{outQueue->get()};
            if (obj != nullptr) obj->unref();
        } //clear out 10 oldest messages
    }
    if (msg != nullptr) {
       msg->ref();
       outQueue->put(msg);
    }
    return true;
}

//------------------------------------------------------------------------------
// clearQueues() -- clear all queues
//------------------------------------------------------------------------------
void Datalink::clearQueues()
{
   base::Object* msg{inQueue->get()};
   while (msg != nullptr) {
      msg->unref();
      msg = inQueue->get();
   }
   msg = outQueue->get();
   while (msg != nullptr) {
      msg->unref();
      msg = outQueue->get();
   }
}


//------------------------------------------------------------------------------
// Event handlers
//------------------------------------------------------------------------------

// DATALINK_MESSAGE event handler
bool Datalink::onDatalinkMessageEvent(base::Object* const msg)
{
   // Just pass it down to all of our subcomponents
   base::PairStream* subcomponents{getComponents()};
   if (subcomponents != nullptr) {
      for (base::List::Item* item = subcomponents->getFirstItem(); item != nullptr; item = item->getNext()) {
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
         base::Component* sc{static_cast<base::Component*>(pair->object())};
         sc->event(DATALINK_MESSAGE, msg);
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }
   return true;
}

bool Datalink::setSlotRadioId(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v{msg->asInt()};
      if (v >= 0 && v <= 0xffff) {
         radioId = static_cast<unsigned short>(v);
         useRadioIdFlg = true;
         ok = true;
      }
   }
   return ok;
}

bool Datalink::setSlotMaxRange(const base::Length* const x)
{
   bool ok{};
   if(x != nullptr) {
      ok = setMaxRange(x->getValueInNauticalMiles());
   }
   return ok;
}

}
}
