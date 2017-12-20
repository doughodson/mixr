
#include "StationTcPeriodicThread.hpp"

#include "mixr/simulation/Station.hpp"

namespace mixr {
namespace simulation {

StationTcPeriodicThread::StationTcPeriodicThread(base::Component* const parent, const double rate): base::PeriodicThread(parent, rate)
{
}

unsigned long StationTcPeriodicThread::userFunc(const double dt)
{
   Station* station{static_cast<Station*>(getParent())};
   station->processTimeCriticalTasks(dt);
   return 0;
}

}
}
