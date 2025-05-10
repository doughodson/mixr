
#include "SimulationBgSyncThread.hpp"

#include "mixr/simulation/ISimulation.hpp"

#include "mixr/base/IComponent.hpp"
#include "mixr/base/PairStream.hpp"

namespace mixr {
namespace simulation {

SimulationBgSyncThread::SimulationBgSyncThread(base::IComponent* const parent): base::ISyncThread(parent)
{
}

void SimulationBgSyncThread::start0(
         base::PairStream* const pl1,
         const double dt1,
         const unsigned int idx1,
         const unsigned int n1
      )
{
   pl0 = pl1;
   dt0 = dt1;
   idx0 = idx1;
   n0 = n1;

   signalStart();
}

unsigned long SimulationBgSyncThread::userFunc()
{
   // Make sure we've a player list and our index is valid ...
   if (pl0 != nullptr && idx0 > 0 && idx0 <= n0) {
      // then call the simulation executives update TC player list functions
      ISimulation* sim{static_cast<ISimulation*>(getParent())};
      sim->updateBgPlayerList(pl0, dt0, idx0, n0);
   }

   return 0;
}

}
}

