
#include "SimulationTcSyncThread.hpp"

#include "mixr/simulation/Simulation.hpp"

#include "mixr/base/Component.hpp"
#include "mixr/base/PairStream.hpp"

namespace mixr {
namespace simulation {

SimulationTcSyncThread::SimulationTcSyncThread(base::Component* const parent): base::SyncThread(parent)
{
}

void SimulationTcSyncThread::start0(
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

unsigned long SimulationTcSyncThread::userFunc()
{
   // Make sure we've a player list and our index is valid ...
   if (pl0 != nullptr && idx0 > 0 && idx0 <= n0) {
      // then call the simulation executives update TC player list functions
      Simulation* sim{static_cast<Simulation*>(getParent())};
      sim->updateTcPlayerList(pl0, dt0, idx0, n0);
   }

   return 0;
}

}
}
