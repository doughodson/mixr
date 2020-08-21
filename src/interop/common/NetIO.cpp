
#include "mixr/interop/common/NetIO.hpp"
#include "mixr/interop/common/Nib.hpp"
#include "mixr/interop/common/Ntm.hpp"

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

#include "mixr/base/util/str_utils.hpp"

#include <cstring>
#include <cmath>
#include <string>

#if !defined(WIN32)
#include <arpa/inet.h>
#endif

namespace mixr {
namespace interop {

IMPLEMENT_ABSTRACT_SUBCLASS(NetIO, "NetIO")

//------------------------------------------------------------------------------
// Parameters
//------------------------------------------------------------------------------
const double NET_TIMEOUT          {12.5};                  //  seconds
const double NET_UPDATE_RATE      {5.0};                   //  seconds
const double NET_THRESHOLD_MTR    {3.0};                   //  meters
const double NET_THRESHOLD_RAD    {3.0 * base::PI/180.0};  //  radians

BEGIN_SLOTTABLE(NetIO)
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
END_SLOTTABLE(NetIO)

BEGIN_SLOT_MAP(NetIO)
   ON_SLOT(1,  setSlotNetworkID,          base::Integer)
   ON_SLOT(2,  setSlotFederationName,     base::Identifier)
   ON_SLOT(3,  setSlotFederateName,       base::Identifier)

   ON_SLOT(4,  setSlotEnableInput,        base::Boolean)
   ON_SLOT(5,  setSlotEnableOutput,       base::Boolean)
   ON_SLOT(6,  setSlotEnableRelay,        base::Boolean)
   ON_SLOT(7,  setSlotTimeline,           base::Identifier)

   ON_SLOT(8,  setSlotInputEntityTypes,   base::PairStream)
   ON_SLOT(9,  setSlotOutputEntityTypes,  base::PairStream)

   ON_SLOT(10, setSlotMaxTimeDR,          base::Time)
   ON_SLOT(11, setSlotMaxPositionErr,     base::Length)
   ON_SLOT(12, setSlotMaxOrientationErr,  base::Angle)
   ON_SLOT(13, setSlotMaxAge,             base::Time)
   ON_SLOT(14, setSlotMaxEntityRange,     base::Length)
END_SLOT_MAP()

NetIO::NetIO()
{
   STANDARD_CONSTRUCTOR()

   // set the defaults
   setMaxEntityRange(0);                       // no range filtering
   setMaxTimeDR(NET_UPDATE_RATE);              //  (seconds)
   setMaxPositionErr(NET_THRESHOLD_MTR);       //  (meters)
   setMaxOrientationErr(NET_THRESHOLD_RAD);    //  (radians)
   setMaxAge(NET_TIMEOUT);                     //  (seconds)
}

void NetIO::copyData(const NetIO& org, const bool)
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

void NetIO::deleteData()
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

void NetIO::reset()
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
bool NetIO::shutdownNotification()
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
double NetIO::getCurrentTime()
{
    if (getTimeline() == UTC) return getSimulation()->getSysTimeOfDay();
    else return getSimulation()->getExecTimeSec();
}

// Entity filter: Returns max entity ranged (meters)
double NetIO::getMaxEntityRange(const Nib* const) const
{
   return maxEntityRange;
}

// Entity filter: Returns max entity ranged squared (meters^2)
double NetIO::getMaxEntityRangeSquared(const Nib* const) const
{
   return maxEntityRange2;
}

// Dead-Reckoning: Returns max DR time before next 'heart beat' (seconds)
double NetIO::getMaxTimeDR(const Nib* const) const
{
   return maxTimeDR;
}

// Dead-Reckoning: Returns max DR position error (meters)
double NetIO::getMaxPositionErr(const Nib* const) const
{
   return maxPositionErr;
}

// Dead-Reckoning: Returns max DR orientation error (radians)
double NetIO::getMaxOrientationErr(const Nib* const) const
{
   return maxOrientationErr;
}

// Dead-Reckoning: Returns max age before a networked player is removed (seconds)
double NetIO::getMaxAge(const Nib* const) const
{
   return maxAge;
}

// Federate name
const std::string& NetIO::getFederateName() const
{
   return federateName;
}

// Federation name
const std::string& NetIO::getFederationName() const
{
   return federationName;
}

//------------------------------------------------------------------------------
// Data set routines
//------------------------------------------------------------------------------

// Sets the Network's ID
bool NetIO::setNetworkID(const unsigned short v)
{
   bool ok{};
   if (v >= 1 && v <= MAX_NETWORD_ID) {
      netID = v;
      ok = true;
   }
   return ok;
}

// Sets the timeline (UTC or EXEC)
bool NetIO::setTimeline(const TSource ts)
{
    timeline = ts;
    return true;
}

// Sets the max dead-rec time; forces next update (sec)
bool NetIO::setMaxTimeDR(const double v)
{
   maxTimeDR = v;
   return true;
}

// Sets the max positional error (meters)
bool NetIO::setMaxPositionErr(const double v)
{
   maxPositionErr = v;
   return true;
}

// Sets the max orientation error (rad)
bool NetIO::setMaxOrientationErr(const double v)
{
   maxOrientationErr = v;
   return true;
}

// Sets the max age; for removal (sec)
bool NetIO::setMaxAge(const double v)
{
   maxAge = v;
   return true;
}

// Sets the max entity range (meters)
bool NetIO::setMaxEntityRange(const double v)
{
   maxEntityRange = v;
   maxEntityRange2 = (v*v);
   return true;
}

// Sets our federate name
bool NetIO::setFederateName(const std::string& x)
{
   federateName = x;
   return true;
}

// Sets our federation name
bool NetIO::setFederationName(const std::string& x)
{
   federationName = x;
   return true;
}

//------------------------------------------------------------------------------
// inputFrame() -- input side of the network
//------------------------------------------------------------------------------
void NetIO::inputFrame(const double)
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
void NetIO::outputFrame(const double)
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
bool NetIO::networkInitialization()
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
void NetIO::cleanupInputList()
{
   // Current exec time
   const double curExecTime{getSimulation()->getExecTimeSec()};

   for (int idx{}; idx < nInNibs; idx++) {
      Nib* nib{inputList[idx]};
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
void NetIO::updateOutputList()
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
                  Nib* nib{findNib(player, OUTPUT_NIB)};
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
void NetIO::processOutputList()
{
   // ---
   // Send player states
   // ---
   for (unsigned int idx{}; idx < getOutputListSize(); idx++) {

      Nib* nib{getOutputNib(idx)};
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
Nib* NetIO::createNewInputNib()
{
    Nib* nib{nibFactory(INPUT_NIB)};
    nib->setNetIO(this);
    return nib;
}

Nib* NetIO::createNewOutputNib(models::Player* const player)
{
   Nib* nib{nibFactory(OUTPUT_NIB)};
   if (nib != nullptr) {
      nib->setNetIO(this);
      nib->setPlayer(player);
      nib->setPlayerID(player->getID());
      nib->setSide(player->getSide());

      // Default DR: World, No rotation, 2nd order linear
      nib->setDeadReckoning(Nib::FVW_DRM);

      std::string fName{getFederateName()};
      if (player->isProxyPlayer()) {
         const auto pNib = dynamic_cast<Nib*>(player->getNib());
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
void NetIO::destroyInputNib(Nib* const nib)
{
   if (nib->getPlayer() != nullptr) {
      // all we really need do is request deletion of the proxy player
      nib->getPlayer()->setMode(models::Player::Mode::DELETE_REQUEST);
   }
   // Once no one has a reference to us, our destructor will be called
   nib->unref();
}

void NetIO::destroyOutputNib(Nib* const nib)
{
   models::Player* p{nib->getPlayer()};
   if (p != nullptr) p->setOutgoingNib(nullptr, netID);

   // Once no one has a reference to us, our destructor will be called.
   nib->unref();
}

//------------------------------------------------------------------------------
// create a new proxy player
//------------------------------------------------------------------------------
models::Player* NetIO::createProxyPlayer(Nib* const nib)
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
Nib* NetIO::insertNewOutputNib(models::Player* const player)
{
    Nib* newNib{};
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
bool NetIO::addNib2InputList(Nib* const nib)
{
    // Only if we allow inputs
    if (!isInputEnabled()) return false;

    return addNibToList(nib, INPUT_NIB);
}

//------------------------------------------------------------------------------
// findNib() -- find the NIB that matches ALL IDs.
//------------------------------------------------------------------------------
Nib* NetIO::findNib(const unsigned short playerID, const std::string& federateName, const IoType ioType)
{
   // Define the key
   NibKey key(playerID, federateName);

   // Binary search the table for the NIB
   Nib* found{};
   if (ioType == INPUT_NIB) {
      Nib** k{static_cast<Nib**>(bsearch(&key, inputList.data(), nInNibs, sizeof(Nib*), compareKey2Nib))};
      if (k != nullptr) found = *k;
   } else {
      Nib** k{static_cast<Nib**>(bsearch(&key, outputList.data(), nOutNibs, sizeof(Nib*), compareKey2Nib))};
      if (k != nullptr) found = *k;
   }
   return found;
}

Nib* NetIO::findNib(const models::Player* const player, const IoType ioType)
{
   Nib* found{};
   if (player != nullptr) {
      // Get the player's IDs
      std::string fName{getFederateName()};
      if (player->isProxyPlayer()) {
         // If networked, used original IDs
         const auto pNib = dynamic_cast<const Nib*>(player->getNib());
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
bool NetIO::addNibToList(Nib* const nib, const IoType ioType)
{
   bool ok{};
   if (nib != nullptr) {
      Nib** tbl{inputList.data()};
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
               Nib* tmp{tbl[idx]};
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
void NetIO::removeNibFromList(Nib* const nib, const IoType ioType)
{
   Nib** tbl{inputList.data()};
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
int NetIO::compareKey2Nib(const void* key, const void* nib)
{

   // The Key
   const NibKey* pKey{static_cast<const NibKey*>(key)};

   // The NIB
   const Nib* const* pp{static_cast<const Nib* const*>(nib)};
   const Nib* pNib{*pp};

   // Default to equal
   int result{};

   // First, compare the player IDs
   if (pKey->id > pNib->getPlayerID()) result = +1;
   else if (pKey->id < pNib->getPlayerID()) result = -1;

   if (result == 0) {
      // If they're the same, compare the federate names
      result = ( pKey->fName == pNib->getFederateName() );
   }

   return result;
}

//------------------------------------------------------------------------------
// Network type mapper functions
//------------------------------------------------------------------------------

// Finds the network type mapper by NIB type codes
const Ntm* NetIO::findNetworkTypeMapper(const Nib* const nib) const
{
   const Ntm* result{};
   if (inputNtmTree != nullptr && nib != nullptr) {
      result = inputNtmTree->findNetworkTypeMapper(nib);
   }
   return result;
}

// Finds the network type mapper by Player
const Ntm* NetIO::findNetworkTypeMapper(const models::Player* const p) const
{
   const Ntm* result{};
   if (outputNtmTree != nullptr && p != nullptr) {
      result = outputNtmTree->findNetworkTypeMapper(p);
   }
   return result;
}

// Adds an item to the input entity type table
bool NetIO::addInputEntityType(Ntm* const ntm)
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
bool NetIO::addOutputEntityType(Ntm* const ntm)
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
bool NetIO::clearInputEntityTypes()
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
bool NetIO::clearOutputEntityTypes()
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
const NetIO::NtmInputNode* NetIO::getRootNtmInputNode() const
{
   return inputNtmTree;
}

// Return the quick look root node for outgoing entity types
const NetIO::NtmOutputNode* NetIO::getRootNtmOutputNode() const
{
   return outputNtmTree;
}

// Return a incoming entity type by index
const Ntm* NetIO::getInputEntityType(const int idx) const
{
   return (idx < nInputEntityTypes) ? inputEntityTypes[idx] : nullptr;
}

// Return a outgoing entity type by index
const Ntm* NetIO::getOutputEntityTypes(const int idx) const
{
   return (idx < nOutputEntityTypes) ? outputEntityTypes[idx] : nullptr;
}

// Number of output types
int NetIO::getNumInputEntityTypes() const
{
   return nInputEntityTypes;
}

// Number of input types
int NetIO::getNumOutputEntityTypes() const
{
   return nOutputEntityTypes;
}

//------------------------------------------------------------------------------
// Slots functions
//------------------------------------------------------------------------------

// Set networkID
bool NetIO::setSlotNetworkID(const base::Integer* const num)
{
    bool ok{};
    if (num != nullptr) {
        const int v{num->asInt()};
        if (v >= 1 && v <= static_cast<int>(MAX_NETWORD_ID)) {
            ok = setNetworkID(static_cast<unsigned short>(v));
        } else {
            std::cerr << "NetIO::setSlotNetworkID(): invalid number(" << v << "); ";
            std::cerr << "valid range:[1 ... " << MAX_NETWORD_ID << " ]" << std::endl;
        }
    }
    return ok;
}

// Sets our federate name
bool NetIO::setSlotFederateName(const base::Identifier* const x)
{
   return setFederateName(x->asString());
}

// Sets our federation name
bool NetIO::setSlotFederationName(const base::Identifier* const x)
{
   return setFederationName(x->asString());
}

// Set input enable flag
bool NetIO::setSlotEnableInput(const base::Boolean* const p)
{
    bool ok{};
    if (p != nullptr) {
        inputFlg = p->asBool();
        ok = true;
    }
    return ok;
}

// Set output enable flag
bool NetIO::setSlotEnableOutput(const base::Boolean* const p)
{
    bool ok{};
    if (p != nullptr) {
        outputFlg = p->asBool();
        ok = true;
    }
    return ok;
}

// Set relay enable flag
bool NetIO::setSlotEnableRelay(const base::Boolean* const p)
{
    bool ok{};
    if (p != nullptr) {
        relayFlg = p->asBool();
        ok = true;
    }
    return ok;
}

// Sets the source of the time ( UTC or EXEC )
bool NetIO::setSlotTimeline(const base::Identifier* const p)
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
bool NetIO::setSlotInputEntityTypes(base::PairStream* const msg)
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
bool NetIO::setSlotOutputEntityTypes(base::PairStream* const msg)
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
bool NetIO::setSlotMaxTimeDR(const base::Time* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setMaxTimeDR(x->getValueInSeconds());
   }
   return ok;
}

// Sets the max positional error(s)
bool NetIO::setSlotMaxPositionErr(const base::Length* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setMaxPositionErr(x->getValueInMeters());
   }
   return ok;
}

// Sets the max orientation error(s)
bool NetIO::setSlotMaxOrientationErr(const base::Angle* const x)
{
   bool ok{};
   if (x != nullptr) {
      const double err{x->getValueInRadians()};
      ok = setMaxOrientationErr( err );
   }
   return ok;
}

// Sets the max age(s)
bool NetIO::setSlotMaxAge(const base::Time* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setMaxAge(x->getValueInSeconds());
   }
   return ok;
}

// Sets the max entity range(s)
bool NetIO::setSlotMaxEntityRange(const base::Length* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setMaxEntityRange(x->getValueInMeters());
   }
   return ok;
}

//==============================================================================
// Class: NtmInputNode
//==============================================================================

IMPLEMENT_PARTIAL_SUBCLASS(NetIO::NtmInputNode, "AbstractNtmInputNode")
EMPTY_SLOTTABLE(NetIO::NtmInputNode)
EMPTY_COPYDATA(NetIO::NtmInputNode)
EMPTY_DELETEDATA(NetIO::NtmInputNode)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
NetIO::NtmInputNode::NtmInputNode()
{
   STANDARD_CONSTRUCTOR()
}

NetIO::NtmInputNode::~NtmInputNode()
{
   STANDARD_DESTRUCTOR()
}

NetIO::NtmInputNode& NetIO::NtmInputNode::operator=(const NetIO::NtmInputNode& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

NetIO::NtmInputNode* NetIO::NtmInputNode::clone() const
{
   return nullptr;
}

//==============================================================================
// Class: NtmOutputNode
//==============================================================================

IMPLEMENT_PARTIAL_SUBCLASS(NetIO::NtmOutputNode, "AbstractNtmOutputNode")
EMPTY_SLOTTABLE(NetIO::NtmOutputNode)
EMPTY_COPYDATA(NetIO::NtmOutputNode)
EMPTY_DELETEDATA(NetIO::NtmOutputNode)

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
NetIO::NtmOutputNode::NtmOutputNode()
{
   STANDARD_CONSTRUCTOR()
}

NetIO::NtmOutputNode::~NtmOutputNode()
{
   STANDARD_DESTRUCTOR()
}

NetIO::NtmOutputNode& NetIO::NtmOutputNode::operator=(const NetIO::NtmOutputNode& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

NetIO::NtmOutputNode* NetIO::NtmOutputNode::clone() const
{
   return nullptr;
}

//==============================================================================
// Class: NtmOutputNodeStd
// Description: Standard outgoing NTM node
//==============================================================================

class NtmOutputNodeStd : public NetIO::NtmOutputNode
{
   DECLARE_SUBCLASS(NtmOutputNodeStd, NetIO::NtmOutputNode)

public:
   NtmOutputNodeStd(const models::Player* const, const char* const factoryName);

   // NetIO::NtmOutputNode class functions
   const Ntm* findNetworkTypeMapper(const models::Player* const) const override;
   bool add2OurLists(Ntm* const) override;

private:
   bool checkAndAddNtm(Ntm* const);
   bool addNtmSorted(Ntm* const);

   char* nodeFactoryName{};     // Factory name for this node
   const models::Player* tp{};  // Template player for this node
   base::List* ntmList{};       // List of Ntm objects at this level
   base::List* subnodeList{};   // List of NtmOutputNode nodes for players derived this level
};

IMPLEMENT_SUBCLASS(NtmOutputNodeStd, "NtmOutputNodeStd")
EMPTY_SLOTTABLE(NtmOutputNodeStd)

//------------------------------------------------------------------------------
// root outgoing NTM node factory
//------------------------------------------------------------------------------
NetIO::NtmOutputNode* NetIO::rootNtmOutputNodeFactory() const
{
   return new NtmOutputNodeStd(nullptr, nullptr);   // root node has no factory name
}

//------------------------------------------------------------------------------
// Class support functions
//------------------------------------------------------------------------------
NtmOutputNodeStd::NtmOutputNodeStd(const models::Player* const p, const char* const name)
{
   STANDARD_CONSTRUCTOR()

   nodeFactoryName = nullptr;
   tp = nullptr;

   if (name != nullptr) {
      const std::size_t LENGTH{std::strlen(name) + 1};
      nodeFactoryName = new char[LENGTH];
      base::utStrcpy(nodeFactoryName,LENGTH,name);
   }

   if (p != nullptr) {
      p->ref();
      tp = p;
}

   ntmList = new base::List();
   subnodeList = new base::List();
}

void NtmOutputNodeStd::copyData(const NtmOutputNodeStd& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      nodeFactoryName = nullptr;
      tp = nullptr;
      ntmList = nullptr;
      subnodeList = nullptr;
   }

   if (nodeFactoryName != nullptr) {
      delete[] nodeFactoryName;
      nodeFactoryName = nullptr;
   }
   if (org.nodeFactoryName != nullptr) {
      const std::size_t LENGTH{std::strlen(org.nodeFactoryName) + 1};
      nodeFactoryName = new char[LENGTH];
      base::utStrcpy(nodeFactoryName,LENGTH,org.nodeFactoryName);
   }

   if (tp != nullptr) {
      tp->unref();
      tp = nullptr;
   }
   if (org.tp != nullptr) {
      tp = org.tp->clone();
   }

   if (ntmList != nullptr) {
      ntmList->unref();
      ntmList = nullptr;
   }
   if (org.ntmList != nullptr) {
      ntmList = org.ntmList->clone();
   }

   if (subnodeList != nullptr) {
      subnodeList->unref();
      subnodeList = nullptr;
   }
   if (org.subnodeList != nullptr) {
      subnodeList = org.subnodeList->clone();
   }

}

void NtmOutputNodeStd::deleteData()
{
   if (nodeFactoryName != nullptr) {
      delete[] nodeFactoryName;
      nodeFactoryName = nullptr;
   }

   if (tp != nullptr) {
      tp->unref();
      tp = nullptr;
   }

   if (ntmList != nullptr) {
      ntmList->unref();
      ntmList = nullptr;
   }

   if (subnodeList != nullptr) {
      subnodeList->unref();
      subnodeList = nullptr;
   }
}

//------------------------------------------------------------------------------
// Find the NTM node for the target player, 'p'.
//------------------------------------------------------------------------------
const Ntm* NtmOutputNodeStd::findNetworkTypeMapper(const models::Player* const p) const
{
   const Ntm* result{};

   // Using factory names, is the target player either the same or derived from
   // our node?  (the root node, which has no factory name, will always match)
   if ( nodeFactoryName == nullptr || p->isFactoryName(nodeFactoryName) ) {

      // First, we'll search our subnodes to see if they'll find a match
      // (i.e., if it's derived from our node then there may be a better match)
      const base::List::Item* item{subnodeList->getFirstItem()};
      while (item != nullptr && result == nullptr) {
         const NtmOutputNodeStd* subnode{static_cast<const NtmOutputNodeStd*>(item->getValue())};
         result = subnode->findNetworkTypeMapper(p);
         item = item->getNext();
      }

      // Second, we'll search our NTM list for a template player with a
      // type string matching the target player.
      if (result == nullptr && nodeFactoryName != nullptr) {

         // Target player's type string and length
         const std::string& pType{p->getType()};
         const std::size_t pTypeLen{pType.length()};

         // Search the NTM for a match with the most matching type string characters,
         // but not more than the target player's type string.
         const base::List::Item* item{ntmList->getFirstItem()};
         while (item != nullptr && result == nullptr) {

            // Get the template player and its type string with length
            const Ntm* tstNtm{static_cast<const Ntm*>(item->getValue())};
            const models::Player* const tp{tstNtm->getTemplatePlayer()};
            const std::string& tpType{tp->getType()};
            const std::size_t tpTypeLen{tpType.length()};

            if (tpTypeLen <= pTypeLen) {
               bool match{(std::strncmp( pType.c_str(), tpType.c_str(), tpTypeLen ) == 0)};
               if (match) {
                  result = tstNtm;
               }
            }
            item = item->getNext();
         }

      }

   }

   return result;
}

//------------------------------------------------------------------------------
// Check if the target NTM's template player is the same or derived from
// this node's player using factory names.
//  1) if factory name match: the Ntm is added to the NTM list, 'ntmList'
//  2) if the target factory is derived from this factory then a new node is created
//  3) if there's not match then nothing changes and false is returned.
//------------------------------------------------------------------------------
bool NtmOutputNodeStd::add2OurLists(Ntm* const tgtNtm)
{
   bool ok{};

   if (tgtNtm != nullptr) {

      ok = true;  // default is true (root node matches all)
      if (nodeFactoryName != nullptr) {
         // Using factory names, check if the target Ntm's
         // template player our form
         const models::Player* const p{tgtNtm->getTemplatePlayer()};
         ok = p->isFactoryName(nodeFactoryName);
      }

      if (ok) {
         // Yes -- check to see if it really belongs to one of our subnodes.
         bool found{};
         base::List::Item* item{subnodeList->getFirstItem()};
         while (item != nullptr && !found) {
            NtmOutputNodeStd* subnode{static_cast<NtmOutputNodeStd*>(item->getValue())};
            found = subnode->add2OurLists(tgtNtm);
            item = item->getNext();
         }

         // If not found then add it to our local lists
         if (!found) ok = checkAndAddNtm(tgtNtm);
      }

   }

   return ok;
}

//------------------------------------------------------------------------------
// Add the target NTM to either (#1) the NTM list or (#2) our list of subnodes.
//------------------------------------------------------------------------------
bool NtmOutputNodeStd::checkAndAddNtm(Ntm* const tgtNtm)
{
   bool ok{};
   if (tgtNtm != nullptr) {

      const models::Player* const tp{tgtNtm->getTemplatePlayer()};
      const std::string& tpfn{tp->getFactoryName()};

      // Case #1 : when the Ntm's template player has the same
      // factory name as this node then we just add it to the list
      // of Ntm objects assigned to this node.
      if (nodeFactoryName != nullptr) {
         ok = ( tpfn == nodeFactoryName );
         if (ok) addNtmSorted(tgtNtm);
      }

      // Case #2 : when the Ntm's template player does not have the
      // same factory name as this node (i.e., it's derived from this
      // node's player class) then create a new NtmOutputNodeStd node for
      // this Ntm object and add it to our list of subnodes.
      if (!ok) {
         // Create a new node and add the NTM
         const auto newNode = new NtmOutputNodeStd(tp, tpfn.c_str());
         newNode->addNtmSorted(tgtNtm);

         // Case #2A : check if any of our subnodes is really a subnode of the new node.
         base::List::Item* item{subnodeList->getFirstItem()};
         while (item != nullptr) {
            const auto subnode = static_cast<NtmOutputNodeStd*>(item->getValue());
            item = item->getNext();

            if ( subnode->tp->isFactoryName( tpfn ) ) {
               // Move it from our subnode list to the new node's subnode list
               newNode->subnodeList->put(subnode);
               this->subnodeList->remove(subnode);
            }

         }

         // add the new node to our subnode list
         subnodeList->put(newNode);
         newNode->unref();
         ok = true;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Add a NTM to the list in sorted order
//------------------------------------------------------------------------------
bool NtmOutputNodeStd::addNtmSorted(Ntm* const newNtm)
{
   bool ok{};
   if (newNtm != nullptr) {
      newNtm->ref();

      // Create a new List::Item to contain this Ntm
      const auto newItem = new base::List::Item();
      newItem->value = newNtm;

      // Get the template player's type string from the 'new' Ntm
      const models::Player* newP{newNtm->getTemplatePlayer()};
      const std::string& newTypeStr{newP->getType()};
      const std::size_t newTypeLen{newTypeStr.length()};

      bool inserted{};
      bool err{};
      base::List::Item* refItem{ntmList->getFirstItem()};
      while (refItem != nullptr && !inserted && !err) {

         // Get the ref player's string from the 'ref' Ntm.
         const Ntm* refNtm{static_cast<const Ntm*>(refItem->getValue())};
         const models::Player* refP{refNtm->getTemplatePlayer()};
         const std::string& refTypeStr{refP->getType()};
         const std::size_t refTypeLen{refTypeStr.length()};

         // compare to the shortest string length
         std::size_t len{newTypeLen};
         if (refTypeLen < len) len = refTypeLen;

         int r{std::strncmp(newTypeStr.c_str(), refTypeStr.c_str(), len)};
         if ( r == 0 && refTypeLen < newTypeLen) {
            ntmList->insert(newItem, refItem);
            inserted = true;
         } else if (r < 0) {
            ntmList->insert(newItem, refItem);
            inserted = true;
         } else if ( r == 0 && refTypeLen == newTypeLen) {
            if (isMessageEnabled(MSG_WARNING)) {
               std::cerr << "Warning: duplicate outgoing NTM( ";
               std::cerr << newP->getFactoryName() << " type: " << newTypeStr;
               std::cerr << " ), second ignored" << std::endl;
               err = true;
            }
            err = true;
         }

         refItem = refItem->getNext();
      }

      if (!inserted && !err) ntmList->insert(newItem,nullptr);

      ok = true;
   }

   return ok;
}

}
}
