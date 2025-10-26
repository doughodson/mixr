
#ifndef __mixr_simulation_SimulationTcSyncThread_HPP__
#define __mixr_simulation_SimulationTcSyncThread_HPP__

#include "mixr/base/threads/ISyncThread.hpp"

namespace mixr {
namespace base { class IComponent; class IPairStream; }
namespace simulation {

//------------------------------------------------------------------------------
// Class: SimulationTcSyncThread
// Description: Simulation time critical synchronized thread
//------------------------------------------------------------------------------
class SimulationTcSyncThread final : public base::ISyncThread
{
public:
   SimulationTcSyncThread(base::IComponent* const parent);

   // Parent thread signals start to this child thread with these parameters.
   void start0(
      base::IPairStream* const pl0,
      const double dt0,
      const unsigned int idx0,
      const unsigned int n0
   );

private:
   // SyncTask class function -- our userFunc()
   unsigned long userFunc() final;

private:
   base::IPairStream* pl0{};
   double dt0{};
   unsigned int idx0{};
   unsigned int n0{};
};

}
}

#endif
