
#include "mixr/interop/INetIO.hpp"
#include "mixr/interop/INib.hpp"
#include "mixr/interop/Ntm.hpp"
#include "mixr/interop/NtmInputNode.hpp"
#include "mixr/interop/NtmOutputNode.hpp"
#include "NtmOutputNodeStd.hpp"

#include "mixr/models/system/Gun.hpp"

#include "mixr/models/player/Player.hpp"

#include "mixr/simulation/Simulation.hpp"
#include "mixr/simulation/Station.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/String.hpp"

#include "mixr/base/numeric/Decibel.hpp"
#include "mixr/base/units/angles.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/units/times.hpp"

#include <cstring>
#include <cmath>

#if !defined(WIN32)
#include <arpa/inet.h>
#endif

namespace mixr {
namespace interop {

IMPLEMENT_ABSTRACT_SUBCLASS(INetIO, "INetIO")

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
const double NET_TIMEOUT          {12.5};                  //  seconds
const double NET_UPDATE_RATE      {5.0};                   //  seconds
const double NET_THRESHOLD_MTR    {3.0};                   //  meters
const double NET_THRESHOLD_RAD    {3.0 * base::PI/180.0};  //  radians

BEGIN_SLOTTABLE(INetIO)
   "networkID",            //  1: Network ID (this interface)
   "federationName",       //  2: Federation name
   "federateName",         //  3: Name of this federate

   "enableInput",          //  4: Input Enable: accept players from this network
   "enableOutput",         //  5: Output enable: send players to this network
   "enableRelay",          //  6: Relay enable: send other network players to this network
   "timeline",             //  7: Source of the timeline { UTC or EXEC }
                           //     (default: UTC)

   "inputEntityTypes",     //  8: Input entity type mappers (Ntm objects that map players to entity types)
   "outputEntityTypes",    //  9: Output entity type mappers (Ntm objects that entity types to players)

   "maxTimeDR",            // 10: Max DR time
   "maxPositionError",     // 11: Max DR position error
   "maxOrientationError",  // 12: Max DR angular error
   "maxAge",               // 13: Max age (without update) of networked players
   "maxEntityRange",       // 14: Max entity range of networked players
END_SLOTTABLE(INetIO)

BEGIN_SLOT_MAP(INetIO)
   ON_SLOT(1,  setSlotNetworkID,          base::Integer)
   ON_SLOT(2,  setSlotFederationName,     base::Identifier)
   ON_SLOT(3,  setSlotFederateName,       base::Identifier)

   ON_SLOT(4,  setSlotEnableInput,        base::Boolean)
   ON_SLOT(5,  setSlotEnableOutput,       base::Boolean)
   ON_SLOT(6,  setSlotEnableRelay,        base::Boolean)
   ON_SLOT(7,  setSlotTimeline,           base::Identifier)

   ON_SLOT(8,  setSlotInputEntityTypes,   base::PairStream)
   ON_SLOT(9,  setSlotOutputEntityTypes,  base::PairStream)

   ON_SLOT(10, setSlotMaxTimeDR,          base::ITime)
   ON_SLOT(11, setSlotMaxPositionErr,     base::ILength)
   ON_SLOT(12, setSlotMaxOrientationErr,  base::IAngle)
   ON_SLOT(13, setSlotMaxAge,             base::ITime)
   ON_SLOT(14, setSlotMaxEntityRange,     base::ILength)
END_SLOT_MAP()

INetIO::INetIO()
{
   STANDARD_CONSTRUCTOR()

   // set the defaults
   setMaxEntityRange(0);                       // no range filtering
   setMaxTimeDR(NET_UPDATE_RATE);              //  (seconds)
   setMaxPositionErr(NET_THRESHOLD_MTR);       //  (meters)
   setMaxOrientationErr(NET_THRESHOLD_RAD);    //  (radians)
   setMaxAge(NET_TIMEOUT);                     //  (seconds)
}

void INetIO::copyData(const INetIO& org, const bool)
{
   BaseClass::copyData(org);

   station = nullptr;
   simulation = nullptr;
   timeline = org.timeline;
   iffEventID = 0;
   emEventID = 0;

   netID = org.netID;

   federateName = org.federateName;
   federationName = org.federationName;

   inputFlg = org.inputFlg;
   outputFlg = org.outputFlg;
   relayFlg = org.relayFlg;
   netInit = org.netInit;
   netInitFail = org.netInitFail;

   setMaxEntityRange(org.maxEntityRange);
   setMaxTimeDR(org.maxTimeDR);
   setMaxPositionErr(org.maxPositionErr);
   setMaxOrientationErr(org.maxOrientationErr);
   setMaxAge(org.maxAge);

   nInNibs = 0;
   nOutNibs = 0;

   clearInputEntityTypes();
   for (int i{}; i < org.nInputEntityTypes; i++) {
      Ntm* cp = org.inputEntityTypes[i]->clone();
      addInputEntityType( cp );
      cp->unref();
   }

    clearOutputEntityTypes();
    for (int i = 0; i < org.nOutputEntityTypes; i++) {
      Ntm* cp = org.outputEntityTypes[i]->clone();
      addOutputEntityType( cp );
      cp->unref();
    }
}

void INetIO::deleteData()
{
   for (int i{}; i < nInNibs; i++) {
      inputList[i]->unref();
      inputList[i] = nullptr;
   }
   nInNibs = 0;

   for (int i{}; i < nOutNibs; i++) {
      outputList[i]->unref();
      outputList[i] = nullptr;
   }
   nOutNibs = 0;

   clearInputEntityTypes();
   clearOutputEntityTypes();

   station = nullptr;
   simulation = nullptr;

   netInit = false;
   netInitFail = false;
}

void INetIO::reset()
{
    // initialization
    if (!isNetworkInitialized() && !didInitializationFail()) {
        networkInitialization();
    }

    //clearTracksAndQueues();
    BaseClass::reset();
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool INetIO::shutdownNotification()
{
    for (int i{}; i < nInNibs; i++) {
        inputList[i]->event(SHUTDOWN_EVENT);
    }

    for (int i{}; i < nOutNibs; i++) {
        outputList[i]->event(SHUTDOWN_EVENT);
    }
    return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// Data access (get) functions
//------------------------------------------------------------------------------

// getCurrentTime() -- Returns the current time from the selected source
double INetIO::getCurrentTime()
{
    if (getTimeline() == UTC) return getSimulation()->getSysTimeOfDay();
    else return getSimulation()->getExecTimeSec();
}

// Entity filter: Returns max entity ranged (meters)
double INetIO::getMaxEntityRange(const INib* const) const
{
   return maxEntityRange;
}

// Entity filter: Returns max entity ranged squared (meters^2)
double INetIO::getMaxEntityRangeSquared(const INib* const) const
{
   return maxEntityRange2;
}

// Dead-Reckoning: Returns max DR time before next 'heart beat' (seconds)
double INetIO::getMaxTimeDR(const INib* const) const
{
   return maxTimeDR;
}

// Dead-Reckoning: Returns max DR position error (meters)
double INetIO::getMaxPositionErr(const INib* const) const
{
   return maxPositionErr;
}

// Dead-Reckoning: Returns max DR orientation error (radians)
double INetIO::getMaxOrientationErr(const INib* const) const
{
   return maxOrientationErr;
}

// Dead-Reckoning: Returns max age before a networked player is removed (seconds)
double INetIO::getMaxAge(const INib* const) const
{
   return maxAge;
}

// Federate name
const std::string& INetIO::getFederateName() const
{
   return federateName;
}

// Federation name
const std::string& INetIO::getFederationName() const
{
   return federationName;
}

//------------------------------------------------------------------------------
// Data set routines
//------------------------------------------------------------------------------

// Sets the Network's ID
bool INetIO::setNetworkID(const unsigned short v)
{
   bool ok{};
   if (v >= 1 && v <= MAX_NETWORK_ID) {
      netID = v;
      ok = true;
   }
   return ok;
}

// Sets the timeline (UTC or EXEC)
bool INetIO::setTimeline(const TSource ts)
{
    timeline = ts;
    return true;
}

// Sets the max dead-rec time; forces next update (sec)
bool INetIO::setMaxTimeDR(const double v)
{
   maxTimeDR = v;
   return true;
}

// Sets the max positional error (meters)
bool INetIO::setMaxPositionErr(const double v)
{
   maxPositionErr = v;
   return true;
}

// Sets the max orientation error (rad)
bool INetIO::setMaxOrientationErr(const double v)
{
   maxOrientationErr = v;
   return true;
}

// Sets the max age; for removal (sec)
bool INetIO::setMaxAge(const double v)
{
   maxAge = v;
   return true;
}

// Sets the max entity range (meters)
bool INetIO::setMaxEntityRange(const double v)
{
   maxEntityRange = v;
   maxEntityRange2 = (v*v);
   return true;
}

// Sets our federate name
bool INetIO::setFederateName(const std::string& x)
{
   federateName = x;
   return true;
}

// Sets our federation name
bool INetIO::setFederationName(const std::string& x)
{
   federationName = x;
   return true;
}

//------------------------------------------------------------------------------
// inputFrame() -- input side of the network
//------------------------------------------------------------------------------
void INetIO::inputFrame(const double)
{
   if (isNetworkInitialized()) {
      netInputHander();     // Input handler
      processInputList();   // Update players/systems from the Input-list
      cleanupInputList();   // Cleanup the Input-List (remove old NABs)
   }
}

//------------------------------------------------------------------------------
// outputFrame() -- output side of the network
//------------------------------------------------------------------------------
void INetIO::outputFrame(const double)
{
   if (isNetworkInitialized()) {
      updateOutputList();   // Update the Output-List from the simulation player list
      processOutputList();  // Create output packets from Output-List
   }
}

//------------------------------------------------------------------------------
// networkInitialization() -- Main network initialization routine
//                            (usually called by updateData())
//------------------------------------------------------------------------------
bool INetIO::networkInitialization()
{
    // reset flags
    netInit = false;
    netInitFail = false;

    // 1) Find our Station
    station = static_cast<simulation::Station*>( findContainerByType(typeid(simulation::Station)) );
    if (station != nullptr) {
        // 2) Find the Simulation
        simulation = station->getSimulation();
        if (simulation != nullptr) {
            // 3) Network specific init
            netInit = initNetwork();
        }
    }

    // Did we fail?
    if (!netInit) {
        station = nullptr;
        simulation = nullptr;
        netInitFail = true;
    }

    return netInit;
}

//------------------------------------------------------------------------------
// cleanupInputList() -- Cleanup the Input-List (remove out of date items)
//------------------------------------------------------------------------------
void INetIO::cleanupInputList()
{
   // Current exec time
   const double curExecTime{getSimulation()->getExecTimeSec()};

   for (int idx{}; idx < nInNibs; idx++) {
      INib* nib{inputList[idx]};
      if ( (nib->isTimeoutEnabled() && ((curExecTime - nib->getTimeExec()) > getMaxAge(nib)) )) {
            // We have one that's timed-out --
            //std::cout << "REMOVED(TO): cur=" << curExecTime << ", NIB=" << nib->getTimeExec() << std::endl;

            // 1) Shift the rest of the list down one
            nInNibs--;
            for (int i{idx}; i < nInNibs; i++) {
               inputList[i] = inputList[i+1];
            }
            inputList[nInNibs] = nullptr;

            // 2) Destroy the NIB
            destroyInputNib(nib);
      } else if (nib->isMode(models::Player::Mode::DELETE_REQUEST) ) {
            // We have one that has a DELETE_REQUEST
            //std::cout << "REMOVED(DR): cur=" << curExecTime << ", NIB=" << nib->getTimeExec() << std::endl;

            // 1) Shift the rest of the list down one
            nInNibs--;
            for (int i{idx}; i < nInNibs; i++) {
               inputList[i] = inputList[i+1];
            }
            inputList[nInNibs] = nullptr;

            // 2) Destroy the NIB
            destroyInputNib(nib);
      }
   }
}

//------------------------------------------------------------------------------
// updateOutputList() --
//   Update the Output-List from the simulation player list (Background thread)
//------------------------------------------------------------------------------
void INetIO::updateOutputList()
{

   if (isNetworkInitialized()) {

      // ---
      // Remove all DELETE_REQUEST mode NIBs
      //   -- The DELETE_REQUEST were issued last pass, so the network
      //       specific software should have handled them by now.
      //   -- As NIBs are removed, the table above the NIB is shifted down.
      //   -- We're also clearing the NIB's 'checked' flag
      // ---
      {
         int i{};  // src index
         int j{};  // dest index
         while (i < nOutNibs) {
            if (outputList[i]->isMode(models::Player::Mode::DELETE_REQUEST)) {
               // Deleting this NIB
               //std::cout << "NetIO::updateOutputList() cleanup: nib = " << outputList[i] << std::endl;
               destroyOutputNib(outputList[i++]);
            } else {
               if (i != j) outputList[j] = outputList[i];
               outputList[j]->setCheckedFlag(false);
               i++;
               j++;
            }
         }
         nOutNibs = j;
      }

      // --- ---
      // Match the main player list with the output-list ...
      // --- ---
      if ( isOutputEnabled() ) {

         // Get the player list pointer (pre-ref()'d)
         base::PairStream* players{getSimulation()->getPlayers()};

         // For all players
         bool finished{};
         unsigned int newCount{};
         base::List::Item* playerItem{players->getFirstItem()};
         while (playerItem != nullptr && !finished) {

            // Get player list items
            base::Pair* playerPair{static_cast<base::Pair*>(playerItem->getValue())};
            models::Player* player{static_cast<models::Player*>(playerPair->object())};

            if (player->isLocalPlayer() || (isRelayEnabled() && player->getNetworkID() != getNetworkID()) )  {
               if ( player->isActive() && player->isNetOutputEnabled()) {

                  // We have (1) an active local player to output or
                  //         (2) an active networked player to relay ...

                  // Find the output NIB for this player
                  INib* nib{findNib(player, OUTPUT_NIB)};
                  if (nib == nullptr && newCount < MAX_NEW_OUTGOING) {
                     // Not Found then create a new output NIB for this player
                     nib = insertNewOutputNib( player );
                     newCount++;
                  }

                  // Mark this NIB as checked
                  if (nib != nullptr) {
                     nib->setCheckedFlag(true);
                  }
               }
            } else {
               // Finished with local players and we're not relaying
               finished = !isRelayEnabled();
            }

            // get the next player
            playerItem = playerItem->getNext();
         }

         players->unref();
      }

      // ---
      // Any NIB that was not checked needs to be removed
      // ---
      for (int i{}; i < nOutNibs; i++) {
         if ( !outputList[i]->isChecked() ) {
            // Request removal;
            // (note: the network specific code now has one frame to cleanup its own code
            //  before the NIB is dropped from the output list next frame -- see above)
            outputList[i]->setMode(models::Player::Mode::DELETE_REQUEST);
         }
      }

   }

}

//------------------------------------------------------------------------------
// processOutputList() -- Process the output-list entities
//------------------------------------------------------------------------------
void INetIO::processOutputList()
{
   // ---
   // Send player states
   // ---
   for (unsigned int idx{}; idx < getOutputListSize(); idx++) {

      INib* nib{getOutputNib(idx)};
      const double curExecTime{getSimulation()->getExecTimeSec()};

      if (nib->isEntityTypeValid()) {

         // While this NIB isn't being deleted ...

         // Check if a fire event is needed.  When a weapon type player is active,
         // but its NIB state is still inactive, then we say the weapon has just
         // been fired. (delay until after the entity state PDU)
         bool fired{
            (nib->isMode(models::Player::Mode::INACTIVE) || nib->isMode(models::Player::Mode::PRE_RELEASE)) &&
            nib->getPlayer()->isMode(models::Player::Mode::ACTIVE) &&
            nib->getPlayer()->isMajorType(models::Player::WEAPON)};

         // Send a detonation message
         if (nib->getPlayer()->isMode(models::Player::Mode::DETONATED) && !nib->wasDetonationMessageSent()) {
            nib->munitionDetonationMsgFactory(static_cast<double>(curExecTime));
         }

         // Manager entity state updates (do this after detonation check because it updates the NIB's mode)
         nib->entityStateManager(static_cast<double>(curExecTime));

         // Send a fire message; if a fire event was needed, we delayed sending
         // until after the weapon's entity state has been sent at least once.
         if (fired) {
            // Weapon player has just gone active and its entity state packet has been sent.
            nib->weaponFireMsgFactory(static_cast<double>(curExecTime));
         }

         // Manage all systems that require network functionality (IFF, Radios, Emitters, etc)
         nib->networkOutputManagers(static_cast<double>(curExecTime));

      }
   }
}

//------------------------------------------------------------------------------
// Create a new NIBs
//------------------------------------------------------------------------------
INib* INetIO::createNewInputNib()
{
    INib* nib{nibFactory(INPUT_NIB)};
    nib->setNetIO(this);
    return nib;
}

INib* INetIO::createNewOutputNib(models::Player* const player)
{
   INib* nib{nibFactory(OUTPUT_NIB)};
   if (nib != nullptr) {
      nib->setNetIO(this);
      nib->setPlayer(player);
      nib->setPlayerID(player->getID());
      nib->setSide(player->getSide());

      // Default DR: World, No rotation, 2nd order linear
      nib->setDeadReckoning(INib::FVW_DRM);

      std::string fName{getFederateName()};
      if (player->isProxyPlayer()) {
         const auto pNib = dynamic_cast<INib*>(player->getNib());
         fName = pNib->getFederateName();
      }
      nib->setFederateName(fName);

      // Maps the player type to an output entity type.
      // Note: isEntityTypeValid() will return false if there
      // isn't a type mapper (Ntb) assigned to this Nib.
      nib->setOutputPlayerType(player);

      // Inform the player
      player->setOutgoingNib(nib, netID);
   }
   return nib;
}

//------------------------------------------------------------------------------
// Destroy the NIBs
//------------------------------------------------------------------------------
void INetIO::destroyInputNib(INib* const nib)
{
   if (nib->getPlayer() != nullptr) {
      // all we really need do is request deletion of the proxy player
      nib->getPlayer()->setMode(models::Player::Mode::DELETE_REQUEST);
   }
   // Once no one has a reference to us, our destructor will be called
   nib->unref();
}

void INetIO::destroyOutputNib(INib* const nib)
{
   models::Player* p{nib->getPlayer()};
   if (p != nullptr) p->setOutgoingNib(nullptr, netID);

   // Once no one has a reference to us, our destructor will be called.
   nib->unref();
}

//------------------------------------------------------------------------------
// create a new proxy player
//------------------------------------------------------------------------------
models::Player* INetIO::createProxyPlayer(INib* const nib)
{
   models::Player* player{};

   // Range filter
   bool inRange{true};
   double maxRng2{getMaxEntityRangeSquared(nib)};
   if (nib != nullptr && maxRng2 > 0) {
      const simulation::Station* sta{getStation()};
      const auto own = dynamic_cast<const models::Player*>(sta->getOwnship());
      if (own != nullptr) {
         base::Vec3d delta{nib->getDrPosition() - own->getGeocPosition()};
         inRange = (delta.length2() <= maxRng2);
      }
   }

   // ---
   // In range and we haven't done a type check yet.
   // ---
   if (inRange && nib != nullptr && nib->isEntityTypeUnchecked()) {

      // Mark that we've been here.
      nib->setEntityTypeChecked( true );

      // Find "by NIB type codes" on the input list
      const Ntm* typeMapper{findNetworkTypeMapper(nib)};

      // ---
      // Clone the 'template' player object (if any)
      // ---
      if (typeMapper != nullptr) {
         const models::Player* templatePlayer{typeMapper->getTemplatePlayer()};
         if (templatePlayer != nullptr) {
            player = templatePlayer->clone();
         }
      }

      // ---
      // Set the player data
      // ---
      if (player != nullptr) {

         // and other stuff ...
         player->container(getSimulation());
         player->setID( nib->getPlayerID() );
         player->setSide( nib->getSide() );
         player->setName( nib->getPlayerName() );
         player->setNib(nib);
         player->setMode(models::Player::Mode::INACTIVE);
         player->setGeocPosition( nib->getDrPosition() );
         player->setGeocEulerAngles( nib->getDrEulerAngles() );
         player->setGeocVelocity( nib->getDrVelocity() );
         player->setGeocAcceleration( nib->getDrAcceleration() );
         player->setAngularVelocities( nib->getDrAngularVelocities() );
         player->setCrashOverride(true);
         nib->setPlayer(player);

         // ---
         // Add it to the player list
         // ---
         const auto playerPair = new base::Pair(nib->getPlayerName().c_str(), player);
         getSimulation()->addNewPlayer(playerPair);

         player->reset();

         // the simulation has it, so we should unref() both the player and the pair.
         player->unref();
         playerPair->unref();
      }
   }

   return player;
}

//------------------------------------------------------------------------------
// insertNewOutputNib() --
//    Create a new Network Interface Block (NIB) for 'player' and insert it
//    in the output list.  Returns a pointer to the new NIB or 0.
//------------------------------------------------------------------------------
INib* INetIO::insertNewOutputNib(models::Player* const player)
{
    INib* newNib{};
    if (player != nullptr) {
        newNib = createNewOutputNib(player);
        if (newNib != nullptr) {
            // Insert the new NIB into the 'output' list
            const bool ok{addNibToList(newNib, OUTPUT_NIB)};
            newNib->unref(); // the list owns it now.
            if (!ok) newNib = nullptr;
        }
    }
    return newNib;
}


//------------------------------------------------------------------------------
// addNib2InputList() --
//    Adds a new NIB to the input-list
//------------------------------------------------------------------------------
bool INetIO::addNib2InputList(INib* const nib)
{
    // Only if we allow inputs
    if (!isInputEnabled()) return false;

    return addNibToList(nib, INPUT_NIB);
}

//------------------------------------------------------------------------------
// findNib() -- find the NIB that matches ALL IDs.
//------------------------------------------------------------------------------
INib* INetIO::findNib(const unsigned short playerID, const std::string& federateName, const IoType ioType)
{
   // Define the key
   NibKey key(playerID, federateName);

   // Binary search the table for the NIB
   INib* found{};
   if (ioType == INPUT_NIB) {
      INib** k{static_cast<INib**>(bsearch(&key, inputList.data(), nInNibs, sizeof(Nib*), compareKey2Nib))};
      if (k != nullptr) found = *k;
   } else {
      INib** k{static_cast<INib**>(bsearch(&key, outputList.data(), nOutNibs, sizeof(Nib*), compareKey2Nib))};
      if (k != nullptr) found = *k;
   }
   return found;
}

INib* INetIO::findNib(const models::Player* const player, const IoType ioType)
{
   INib* found{};
   if (player != nullptr) {
      // Get the player's IDs
      std::string fName{getFederateName()};
      if (player->isProxyPlayer()) {
         // If networked, used original IDs
         const auto pNib = dynamic_cast<const INib*>(player->getNib());
         fName = pNib->getFederateName();
      }
      // Now find the NIB using the player's IDs
      found = findNib(player->getID(), fName, ioType);
   }
   return found;
}

//------------------------------------------------------------------------------
// addNibToList() -- adds a NIB to the quick access table
//------------------------------------------------------------------------------
bool INetIO::addNibToList(INib* const nib, const IoType ioType)
{
   bool ok{};
   if (nib != nullptr) {
      INib** tbl{inputList.data()};
      int n{nInNibs};
      if (ioType == OUTPUT_NIB) {
         tbl = outputList.data();
         n = nOutNibs;
      }

      if (n < MAX_OBJECTS) {

         // Put the NIB on the top of the table
         nib->ref();
         tbl[n] = nib;

         // Create a key for this new NIB
         NibKey key(nib->getPlayerID(), nib->getFederateName());

         if (n > 0) {
            // Now, 'bubble down' to its correct position
            int idx{n-1};
            while (idx >= 0 && compareKey2Nib(&key, &tbl[idx]) <= 0) {
               // Swap the table entries
               INib* tmp{tbl[idx]};
               tbl[idx] = tbl[idx+1];
               tbl[idx+1] = tmp;
               idx--;
            }
         }

         // Increment the count
         if (ioType == OUTPUT_NIB) nOutNibs++;
         else nInNibs++;

         ok = true;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// removeNibFromList() -- removes a NIB from the quick access table
//------------------------------------------------------------------------------
void INetIO::removeNibFromList(INib* const nib, const IoType ioType)
{
   INib** tbl{inputList.data()};
   int n{nInNibs};
   if (ioType == OUTPUT_NIB) {
      tbl = outputList.data();
      n = nOutNibs;
   }

   int found{-1};
   // Find the NIB
   for (int i{}; i < n && found < 0; i++) {
      if (nib == tbl[i]) found = i;
   }

   // Shift down all items above this NIB one position
   if (found >= 0) {
      tbl[found]->unref();
      int n1{n - 1};
      for (int i{found}; i < n1; i++) {
         tbl[i] = tbl[i+1];
      }
      tbl[n-1] = nullptr;

      // Decrement the count
      if (ioType == OUTPUT_NIB) --nOutNibs;
      else --nInNibs;
   }
}

//------------------------------------------------------------------------------
// bsearch callbacks: object name compare function --
//   True types are (const NibKey* key, const Nib** nib)
//------------------------------------------------------------------------------
int INetIO::compareKey2Nib(const void* key, const void* nib)
{
   // the key
   const NibKey* pKey{static_cast<const NibKey*>(key)};

   // the NIB
   const INib* const* pp{static_cast<const INib* const*>(nib)};
   const INib* pNib{*pp};

   // default to equal
   int result{};

   // first, compare the player IDs
   if (pKey->id > pNib->getPlayerID()) result = +1;
   else if (pKey->id < pNib->getPlayerID()) result = -1;

   if (result == 0) {
      // if they're the same, compare the federate names
      result = std::strcmp(pKey->fName.c_str(), pNib->getFederateName().c_str());
   }

   return result;
}

//------------------------------------------------------------------------------
// Network type mapper functions
//------------------------------------------------------------------------------

// Finds the network type mapper by NIB type codes
const Ntm* INetIO::findNetworkTypeMapper(const INib* const nib) const
{
   const Ntm* result{};
   if (inputNtmTree != nullptr && nib != nullptr) {
      result = inputNtmTree->findNetworkTypeMapper(nib);
   }
   return result;
}

// Finds the network type mapper by Player
const Ntm* INetIO::findNetworkTypeMapper(const models::Player* const p) const
{
   const Ntm* result{};
   if (outputNtmTree != nullptr && p != nullptr) {
      result = outputNtmTree->findNetworkTypeMapper(p);
   }
   return result;
}

// Adds an item to the input entity type table
bool INetIO::addInputEntityType(Ntm* const ntm)
{
   bool ok{};
   if (nInputEntityTypes < MAX_ENTITY_TYPES && ntm != nullptr) {

      ntm->ref();
      inputEntityTypes[nInputEntityTypes] = ntm;
      nInputEntityTypes++;

      // Make sure we have a root node for the quick look tree ...
      if (inputNtmTree == nullptr) {
         inputNtmTree = rootNtmInputNodeFactory();
      }

      // Add to the quick look tree
      if (inputNtmTree != nullptr) {
         ok = inputNtmTree->add2OurLists(ntm);
      }

   }
   return ok;
}

// Adds an item to the output entity type table
bool INetIO::addOutputEntityType(Ntm* const ntm)
{
   bool ok{};
   if (nOutputEntityTypes < MAX_ENTITY_TYPES && ntm != nullptr) {

      ntm->ref();
      outputEntityTypes[nOutputEntityTypes] = ntm;
      nOutputEntityTypes++;

      // Make sure we have a root node ...
      if (outputNtmTree == nullptr) {
         outputNtmTree = rootNtmOutputNodeFactory();
      }

      // add to the root node
      if (outputNtmTree != nullptr) {
         ok = outputNtmTree->add2OurLists(ntm);
      }
   }

   return ok;
}

// Clears the input entity type table
bool INetIO::clearInputEntityTypes()
{
   // Unref() the root node of the quick look tree
   if (inputNtmTree != nullptr) {
      inputNtmTree->unref();
      inputNtmTree = nullptr;
   }

   // Clear our old input entity type table --
   while (nInputEntityTypes > 0) {
      nInputEntityTypes--;
      inputEntityTypes[nInputEntityTypes]->unref();
      inputEntityTypes[nInputEntityTypes] = nullptr;
   }

   return true;
}

// Clears the output entity type table
bool INetIO::clearOutputEntityTypes()
{
   // Unref() the root node of the quick look tree
   if (outputNtmTree != nullptr) {
      outputNtmTree->unref();
      outputNtmTree = nullptr ;
   }

   // Clear our old output entity type table --
   while (nOutputEntityTypes > 0) {
      nOutputEntityTypes--;
      outputEntityTypes[nOutputEntityTypes]->unref();
      outputEntityTypes[nOutputEntityTypes] = nullptr;
   }

   return true;
}

// Return the quick look root node for incoming entity types
const NtmInputNode* INetIO::getRootNtmInputNode() const
{
   return inputNtmTree;
}

// Return the quick look root node for outgoing entity types
const NtmOutputNode* INetIO::getRootNtmOutputNode() const
{
   return outputNtmTree;
}

// Return a incoming entity type by index
const Ntm* INetIO::getInputEntityType(const int idx) const
{
   return (idx < nInputEntityTypes) ? inputEntityTypes[idx] : nullptr;
}

// Return a outgoing entity type by index
const Ntm* INetIO::getOutputEntityTypes(const int idx) const
{
   return (idx < nOutputEntityTypes) ? outputEntityTypes[idx] : nullptr;
}

// Number of output types
int INetIO::getNumInputEntityTypes() const
{
   return nInputEntityTypes;
}

// Number of input types
int INetIO::getNumOutputEntityTypes() const
{
   return nOutputEntityTypes;
}

//------------------------------------------------------------------------------
// Slots functions
//------------------------------------------------------------------------------

// Set networkID
bool INetIO::setSlotNetworkID(const base::Integer* const num)
{
    bool ok{};
    if (num != nullptr) {
        const int v{num->asInt()};
        if (v >= 1 && v <= static_cast<int>(MAX_NETWORK_ID)) {
            ok = setNetworkID(static_cast<unsigned short>(v));
        } else {
            std::cerr << "NetIO::setSlotNetworkID(): invalid number(" << v << "); ";
            std::cerr << "valid range:[1 ... " << MAX_NETWORK_ID << " ]" << std::endl;
        }
    }
    return ok;
}

// Sets our federate name
bool INetIO::setSlotFederateName(const base::Identifier* const x)
{
   return setFederateName(x->asString());
}

// Sets our federation name
bool INetIO::setSlotFederationName(const base::Identifier* const x)
{
   return setFederationName(x->asString());
}

// Set input enable flag
bool INetIO::setSlotEnableInput(const base::Boolean* const p)
{
    bool ok{};
    if (p != nullptr) {
        inputFlg = p->asBool();
        ok = true;
    }
    return ok;
}

// Set output enable flag
bool INetIO::setSlotEnableOutput(const base::Boolean* const p)
{
    bool ok{};
    if (p != nullptr) {
        outputFlg = p->asBool();
        ok = true;
    }
    return ok;
}

// Set relay enable flag
bool INetIO::setSlotEnableRelay(const base::Boolean* const p)
{
    bool ok{};
    if (p != nullptr) {
        relayFlg = p->asBool();
        ok = true;
    }
    return ok;
}

// Sets the source of the time ( UTC or EXEC )
bool INetIO::setSlotTimeline(const base::Identifier* const p)
{
    bool ok{};
    if (p != nullptr) {
        if (*p == "EXEC") {
            setTimeline( EXEC );
            ok = true;
        } else if (*p == "UTC") {
            setTimeline( UTC );
            ok = true;
        }
    }
    return ok;
}

// Sets the table of input entity to player mapper objects
bool INetIO::setSlotInputEntityTypes(base::PairStream* const msg)
{
    bool ok{};
    if (msg != nullptr) {
       // First clear the old table
       clearInputEntityTypes();

       // Now scan the pair stream and put all Ntm objects into the table.
       base::List::Item* item{msg->getFirstItem()};
       while (item != nullptr) {
          const auto pair = static_cast<base::Pair*>(item->getValue());
          const auto ntm = dynamic_cast<Ntm*>( pair->object() );
          if (ntm != nullptr) {
             // We have an Ntm object, so put it in the table
             addInputEntityType(ntm);
          }
          item = item->getNext();
       }
       ok = true;
    }

    return ok;
}

// Sets the table of output entity to player mapper objects
bool INetIO::setSlotOutputEntityTypes(base::PairStream* const msg)
{
    bool ok{};
    if (msg != nullptr) {
       // First clear the old table
       clearOutputEntityTypes();

       // Now scan the pair stream and put all Ntm objects into the table.
       base::List::Item* item{msg->getFirstItem()};
       while (item != nullptr) {
          const auto pair = static_cast<base::Pair*>(item->getValue());
          const auto ntm = dynamic_cast<Ntm*>( pair->object() );
          if (ntm != nullptr) {
            // We have an Ntm object, so put it in the table
            addOutputEntityType(ntm);
          }
          item = item->getNext();
       }
       ok = true;
    }

    return ok;
}

// Sets the mac DR time(s)
bool INetIO::setSlotMaxTimeDR(const base::ITime* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setMaxTimeDR(x->getValueInSeconds());
   }
   return ok;
}

// Sets the max positional error(s)
bool INetIO::setSlotMaxPositionErr(const base::ILength* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setMaxPositionErr(x->getValueInMeters());
   }
   return ok;
}

// Sets the max orientation error(s)
bool INetIO::setSlotMaxOrientationErr(const base::IAngle* const x)
{
   bool ok{};
   if (x != nullptr) {
      const double err{x->getValueInRadians()};
      ok = setMaxOrientationErr( err );
   }
   return ok;
}

// Sets the max age(s)
bool INetIO::setSlotMaxAge(const base::ITime* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setMaxAge(x->getValueInSeconds());
   }
   return ok;
}

// Sets the max entity range(s)
bool INetIO::setSlotMaxEntityRange(const base::ILength* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setMaxEntityRange(x->getValueInMeters());
   }
   return ok;
}

//------------------------------------------------------------------------------
// root outgoing NTM node factory
//------------------------------------------------------------------------------
NtmOutputNode* INetIO::rootNtmOutputNodeFactory() const
{
   return new NtmOutputNodeStd(nullptr, nullptr);   // root node has no factory name
}

}
}
