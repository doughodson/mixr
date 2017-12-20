
#ifndef __mixr_simulation_SimulationTcSyncThread_H__
#define __mixr_simulation_SimulationTcSyncThread_H__

#include "mixr/base/threads/SyncThread.hpp"

namespace mixr {
namespace base { class Component; class PairStream; }
namespace simulation {

//------------------------------------------------------------------------------
// Class: SimulationTcSyncThread
// Description: Simulation time critical synchronized thread
//------------------------------------------------------------------------------
class SimulationTcSyncThread final : public base::SyncThread
{
public:
   SimulationTcSyncThread(base::Component* const parent);

   // Parent thread signals start to this child thread with these parameters.
   void start0(
      base::PairStream* const pl0,
      const double dt0,
      const unsigned int idx0,
      const unsigned int n0
   );

private:
   // SyncTask class function -- our userFunc()
   unsigned long userFunc() final;

private:
   base::PairStream* pl0{};
   double dt0{};
   unsigned int idx0{};
   unsigned int n0{};
};

}
}

#endif
