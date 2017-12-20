
#include "StationNetPeriodicThread.hpp"

#include "mixr/simulation/Station.hpp"

namespace mixr {
namespace simulation {

StationNetPeriodicThread::StationNetPeriodicThread(base::Component* const parent, const double rate): base::PeriodicThread(parent, rate)
{
}

unsigned long StationNetPeriodicThread::userFunc(const double dt)
{
   Station* station{static_cast<Station*>(getParent())};
   station->processNetworkInputTasks(dt);
   station->processNetworkOutputTasks(dt);
   return 0;
}

}
}
