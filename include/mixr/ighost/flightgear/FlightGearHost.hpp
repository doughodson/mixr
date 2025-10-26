
#ifndef __mixr_ighost_flightgear_FlightGearHost_HPP__
#define __mixr_ighost_flightgear_FlightGearHost_HPP__

#include "mixr/simulation/IIgHost.hpp"

namespace mixr {
namespace base { class INetHandler; class IPairStream; }
namespace models { class IPlayer; }
namespace ighost {
namespace flightgear {

//------------------------------------------------------------------------------
// Class: FlightGearHost
//------------------------------------------------------------------------------
class FlightGearHost final: public simulation::IIgHost
{
   DECLARE_SUBCLASS(FlightGearHost, simulation::IIgHost)

public:
   FlightGearHost();

   // sets our ownship and player list pointers, used by Station class
   void setOwnship(simulation::IPlayer* const) final;
   void setPlayerList(base::IPairStream* const newPlayerList) final;

   void reset() final;

private:
   void updateIg(const double dt = 0.0) override;
   void frameSync();

   bool isNetworkInitialized() const       { return netInit; }
   bool didInitializationFail() const      { return netInitFail; }

   void sendData();                        // send data block
   bool initNetwork();                     // initialize network connection

   base::safe_ptr<base::INetHandler> netOutput;  // output network handler
   bool netInit{};                              // network has been initialized
   bool netInitFail{};                          // initialization attempt failed
   int scnt{};                                  // send timer

   void setOwnship0(models::IPlayer* const);     // sets our ownship player

   // simulation inputs
   models::IPlayer* ownship{};              // current ownship
   base::IPairStream* playerList{};         // current player list

private:
   // slot table helper methods
   bool setSlotNetOutput(base::INetHandler* const);
};

}
}
}

#endif
