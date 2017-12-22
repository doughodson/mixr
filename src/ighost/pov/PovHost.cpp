
#include "mixr/ighost/pov/PovHost.hpp"

#include "mixr/ighost/pov/EntityState.hpp"

#include "mixr/models/player/air/AirVehicle.hpp"
#include "mixr/models/player/Player.hpp"

#include "mixr/base/network/NetHandler.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/osg/Vec3d"

namespace mixr {
namespace pov {

IMPLEMENT_SUBCLASS(PovHost, "PovHost")

BEGIN_SLOTTABLE(PovHost)
   "netOutput",        // 1) Network output handler
END_SLOTTABLE(PovHost)

BEGIN_SLOT_MAP(PovHost)
   ON_SLOT(1, setSlotNetOutput, base::NetHandler)
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
void PovHost::setOwnship(simulation::AbstractPlayer* const newOwnship)
{
   const auto player = dynamic_cast<models::Player*>(newOwnship);
   if (player != nullptr) {
      setOwnship0(player);
   }
}

//------------------------------------------------------------------------------
// Sets our ownship player (for derived class control)
//------------------------------------------------------------------------------
void PovHost::setOwnship0(models::Player* const newOwnship)
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
   //const double DEG2MR = (PI / 180.0f * 1000.0f);

   // Ownship type air vehicle?
   const auto av = dynamic_cast<const models::AirVehicle*>(ownship);
   if (av != nullptr) {

      EntityState entityState;

      entityState.x_cg = 0;
      entityState.y_cg = 0;
      entityState.z_cg = 0;          // altitude

      const base::Vec3d pos{av->getPosition()};
      entityState.pilot_eye_x = pos[0] * base::distance::M2FT;
      entityState.pilot_eye_y = pos[1] * base::distance::M2FT;
      entityState.pilot_eye_z = -pos[2] * base::distance::M2FT;   // altitude

      entityState.alphad = av->getAngleOfAttackD();
      entityState.betad = av->getSideSlipD();
      entityState.mach = av->getMach();
      entityState.runalt = 0.0;

      entityState.theta    = static_cast<float>(av->getPitchD());
      entityState.phi      = static_cast<float>(av->getRollD());
      entityState.psi      = static_cast<float>(av->getHeadingD());
      entityState.airspeed = static_cast<float>(av->getTotalVelocityKts());

      entityState.heading = static_cast<float>(av->getHeadingD());

      entityState.dlg = 0;           // landing gear position 90 is down (scaled to 0-1)
      entityState.dsb = static_cast<float>(av->getSpeedBrakePosition()/100.0f);   // speed break 60 is out (scaled to 0-1)
      entityState.nz  = static_cast<float>(av->getGload());

      entityState.aetrc = 0;         // Commanded throttle position
      entityState.afterburner = 0;   // logical, true in in A/B

      entityState.target_id = 0;

      entityState.id_self = 0;       // make use of a hole
      entityState.flags = 0;

      entityState.target_x = 0;
      entityState.target_y = 0;
      entityState.target_z = 0;

      entityState.target_theta = 0;
      entityState.target_phi = 0;
      entityState.target_psi = 0;

      entityState.target_uearth = 0;
      entityState.target_vearth = 0;
      entityState.target_wearth = 0;
      entityState.target_vcas = 0;

      if (netOutput != nullptr) {
         netOutput->sendData( reinterpret_cast<char*>(&entityState), sizeof(entityState) );
      }
   }
}

//------------------------------------------------------------------------------
// Set Slot Functions
//------------------------------------------------------------------------------

bool PovHost::setSlotNetOutput(base::NetHandler* const msg)
{
   netOutput = msg;
   return true;
}

}
}

