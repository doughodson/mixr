
#include "mixr/ighost/flightgear/FlightGearHost.hpp"

#include "mixr/base/util/endian_utils.hpp"
#include "mixr/ighost/flightgear/swap_endian.hpp"
#include "mixr/ighost/flightgear/FGNetFDM.hpp"

#include "mixr/models/player/air/AirVehicle.hpp"
#include "mixr/models/player/Player.hpp"

#include "mixr/base/network/NetHandler.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/units/util/angle_utils.hpp"

#include <ctime>
#include <iostream>

namespace mixr {
namespace flightgear {

IMPLEMENT_SUBCLASS(FlightGearHost, "FlightGearHost")

BEGIN_SLOTTABLE(FlightGearHost)
   "netOutput",        // 1) Network output handler
END_SLOTTABLE(FlightGearHost)

BEGIN_SLOT_MAP(FlightGearHost)
   ON_SLOT(1, setSlotNetOutput, base::NetHandler)
END_SLOT_MAP()

FlightGearHost::FlightGearHost()
{
   STANDARD_CONSTRUCTOR()
}

void FlightGearHost::copyData(const FlightGearHost& org, const bool cc)
{
   BaseClass::copyData(org);

   setOwnship(org.ownship);
   setPlayerList(org.playerList);

   netOutput = nullptr;
   netInit = org.netInit;
   netInitFail = org.netInitFail;
   scnt = 0;
}

void FlightGearHost::deleteData()
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
void FlightGearHost::reset()
{
   BaseClass::reset();
   setPlayerList(nullptr);
}

//------------------------------------------------------------------------------
// setPlayerList() -- Sets our player list pointer
//------------------------------------------------------------------------------
void FlightGearHost::setPlayerList(base::PairStream* const newPlayerList)
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
void FlightGearHost::setOwnship(simulation::AbstractPlayer* const newOwnship)
{
   const auto player = dynamic_cast<models::Player*>(newOwnship);
   if (player != nullptr) {
      setOwnship0(player);
   }
}

//------------------------------------------------------------------------------
// Sets our ownship player (for derived class control)
//------------------------------------------------------------------------------
void FlightGearHost::setOwnship0(models::Player* const newOwnship)
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
void FlightGearHost::updateIg(const double)
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
void FlightGearHost::frameSync()
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

bool FlightGearHost::initNetwork()
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
         std::cerr << "FlightGearHost::initNetwork(): failure to find the network output handler (see slot 'netOutput')" << std::endl;
      }
      ok = false;
   }

   return ok;
}

void FlightGearHost::sendData()
{
   // ownship type air vehicle?
   const auto av = dynamic_cast<const models::AirVehicle*>(ownship);
   if (av != nullptr) {

      FGNetFDM fgNetFDM;

      // positions
      fgNetFDM.latitude = av->getLatitude() * base::angle::D2RCC;
      fgNetFDM.longitude = av->getLongitude() * base::angle::D2RCC;
      fgNetFDM.altitude = av->getAltitude();
      fgNetFDM.agl - av->getAltitudeAgl();
      fgNetFDM.phi   = static_cast<float>(av->getRollD() * base::angle::D2RCC);
      fgNetFDM.theta = static_cast<float>(av->getPitchD() * base::angle::D2RCC);
      fgNetFDM.psi   = static_cast<float>(av->getHeadingD() * base::angle::D2RCC);
      fgNetFDM.alpha = static_cast<float>(av->getAngleOfAttackD() * base::angle::D2RCC);
      fgNetFDM.beta  = static_cast<float>(av->getSideSlipD() * base::angle::D2RCC);

      if (!base::is_big_endian()) {
         swap_endian(&fgNetFDM);
      }

      if (netOutput != nullptr) {
         netOutput->sendData(reinterpret_cast<char*>(&fgNetFDM), sizeof(fgNetFDM));
      }
   }
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

bool FlightGearHost::setSlotNetOutput(base::NetHandler* const msg)
{
   netOutput = msg;
   return true;
}

}
}

