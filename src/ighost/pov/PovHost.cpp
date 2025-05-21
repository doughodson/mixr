
#include "mixr/ighost/pov/PovHost.hpp"

#include "mixr/base/util/endian_utils.hpp"
#include "mixr/ighost/pov/swap_endian.hpp"
#include "mixr/ighost/pov/Pov.hpp"

#include "mixr/models/player/air/AirVehicle.hpp"
#include "mixr/models/player/IPlayer.hpp"

#include "mixr/base/network/INetHandler.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace ighost {
namespace pov {

IMPLEMENT_SUBCLASS(PovHost, "PovHost")

BEGIN_SLOTTABLE(PovHost)
   "netOutput",        // 1) Network output handler
END_SLOTTABLE(PovHost)

BEGIN_SLOT_MAP(PovHost)
   ON_SLOT(1, setSlotNetOutput, base::INetHandler)
END_SLOT_MAP()

PovHost::PovHost()
{
   STANDARD_CONSTRUCTOR()
}

void PovHost::copyData(const PovHost& org, const bool cc)
{
   BaseClass::copyData(org);

   setOwnship(org.ownship);
   setPlayerList(org.playerList);

   netOutput = nullptr;
   netInit = org.netInit;
   netInitFail = org.netInitFail;
   scnt = 0;
}

void PovHost::deleteData()
{
   setOwnship(nullptr);
   setPlayerList(nullptr);

   netOutput = nullptr;
   netInit = false;
   netInitFail = false;
}

//------------------------------------------------------------------------------
// reset() -- Reset the visual system interface
//------------------------------------------------------------------------------
void PovHost::reset()
{
   BaseClass::reset();
   setPlayerList(nullptr);
}

//------------------------------------------------------------------------------
// setPlayerList() -- Sets our player list pointer
//------------------------------------------------------------------------------
void PovHost::setPlayerList(base::PairStream* const newPlayerList)
{
    // Nothing's changed, just return
    if (playerList == newPlayerList) return;

    // Unref() the old, set and ref() the new
    if (playerList != nullptr) playerList->unref();
    playerList = newPlayerList;
    if (playerList != nullptr) playerList->ref();
}

//------------------------------------------------------------------------------
// Sets our ownship pointer; public version, which is usually called by
// the Station class.
//------------------------------------------------------------------------------
void PovHost::setOwnship(simulation::IPlayer* const newOwnship)
{
   const auto player = dynamic_cast<models::IPlayer*>(newOwnship);
   if (player != nullptr) {
      setOwnship0(player);
   }
}

//------------------------------------------------------------------------------
// Sets our ownship player (for derived class control)
//------------------------------------------------------------------------------
void PovHost::setOwnship0(models::IPlayer* const newOwnship)
{
    // Nothing's changed, just return
    if (ownship == newOwnship) return;

    // Unref() the old, set and ref() the new
    if (ownship != nullptr) ownship->unref();
    ownship = newOwnship;
    if (ownship != nullptr) ownship->ref();
}

//------------------------------------------------------------------------------
// Send state data for ownship to IG
//------------------------------------------------------------------------------
void PovHost::updateIg(const double)
{
   frameSync();

   scnt--;
   if (scnt <= 0) {
      sendData();
      scnt = 5;   // send every 5th frame
   }
}

//------------------------------------------------------------------------------
// triggers the frame update
//------------------------------------------------------------------------------
void PovHost::frameSync()
{
   // initialization
   if (!isNetworkInitialized() && !didInitializationFail()) {
      if (initNetwork()) {
         netInit = true;
         netInitFail = false;
      } else {
         netInit = false;
         netInitFail = true;
      }
   }
}

bool PovHost::initNetwork()
{
   bool ok{true};

   // initialize network output handler
   if (netOutput != nullptr) {
      if (netOutput->initNetwork(true)) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "netOutput Initialize OK" << std::endl;
         }
      }
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "PovHost::initNetwork(): failure to find the network output handler (see slot 'netOutput')" << std::endl;
      }
      ok = false;
   }

   return ok;
}

void PovHost::sendData()
{
   // Ownship type air vehicle?
   const auto av = dynamic_cast<const models::AirVehicle*>(ownship);
   if (av != nullptr) {

      Pov pov;

      const base::Vec3d pos{av->getPosition()};
      pov.north = pos[0];
      pov.east = pos[1];
      pov.alt_agl = av->getAltitudeAgl();  // altitude (AGL) in meters

      pov.roll = av->getRollD();
      pov.pitch = av->getPitchD();
      pov.heading = av->getHeadingD();

      //std::cout << "north:   " << pov.north   << " meters" << std::endl;
      //std::cout << "east:    " << pov.east    << " meters" << std::endl;
      //std::cout << "alt_agl: " << pov.alt_agl << " meters" << std::endl;
      //std::cout << "roll:    " << pov.roll    << " degrees" << std::endl;
      //std::cout << "pitch:   " << pov.pitch   << " degrees" << std::endl;
      //std::cout << "heading: " << pov.heading << " degrees" << std::endl;
      //std::cout << std::endl;

      if (!base::is_big_endian()) {
         swap_endian(&pov);
      }
      if (netOutput != nullptr) {
         netOutput->sendData( reinterpret_cast<char*>(&pov), sizeof(pov) );
      }
   }
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

bool PovHost::setSlotNetOutput(base::INetHandler* const x)
{
   netOutput = x;
   return true;
}

}
}
}

