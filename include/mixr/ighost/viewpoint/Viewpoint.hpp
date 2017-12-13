
#ifndef __mixr_ighost_viewpoint_Viewpoint_H__
#define __mixr_ighost_viewpoint_Viewpoint_H__

#include "mixr/simulation/AbstractIgHost.hpp"

namespace mixr {
namespace base { class NetHandler; }
namespace models { class Player; }
namespace viewpoint {

//------------------------------------------------------------------------------
// Class: Viewpoint
//------------------------------------------------------------------------------
class Viewpoint : public simulation::AbstractIgHost
{
   DECLARE_SUBCLASS(Viewpoint, simulation::AbstractIgHost)

public:
   Viewpoint();

   const models::Player* getOwnship() const              { return ownship; }        // Our ownship -- the player that we're following

   void setPlayerList(base::PairStream* const newPlayerList) override; // Sets the player list that we're to use to generate player/models

   // Sets our ownship pointer; public version, which is usually called by the Station class.  Derived classes
   // can override this function and control the switching of the ownship using setOwnship0()
   void setOwnship(simulation::AbstractPlayer* const) override;

   void reset() override;

private:
   void updateIg(const double dt = 0.0) override;
   void frameSync();

   bool isNetworkInitialized() const       { return netInit; }
   bool didInitializationFail() const      { return netInitFail; }

   void sendData();                        // send data block
   bool initNetwork();                     // initialize network connection

   base::safe_ptr<base::NetHandler> netOutput;  // output network handler
   bool netInit {};                             // network has been initialized
   bool netInitFail {};                         // initialization attempt failed
   int scnt {};                                 // send timer

   void setOwnship0(models::Player* const);     // sets our ownship player

   // Simulation inputs
   models::Player* ownship {};             // current ownship
   base::PairStream* playerList {};        // current player list

private:
   // slot table helper methods
   bool setSlotNetOutput(base::NetHandler* const);
};

}
}

#endif
