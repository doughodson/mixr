
#ifndef __mixr_simulation_SimulationBgSyncThread_HPP__
#define __mixr_simulation_SimulationBgSyncThread_HPP__

#include "mixr/base/threads/SyncThread.hpp"

namespace mixr {
namespace base { class IComponent; class PairStream; }
namespace simulation {

//------------------------------------------------------------------------------
// Class: SimulationBgSyncThread
// Description: Simulation background synchronized thread
//------------------------------------------------------------------------------
class SimulationBgSyncThread final : public base::SyncThread
{
public:
   SimulationBgSyncThread(base::IComponent* const parent);

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
