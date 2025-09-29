
#include "StationTcPeriodicThread.hpp"

#include "mixr/simulation/IStation.hpp"

namespace mixr {
namespace simulation {

StationTcPeriodicThread::StationTcPeriodicThread(base::IComponent* const parent, const double rate): base::IPeriodicThread(parent, rate)
{
}

unsigned long StationTcPeriodicThread::userFunc(const double dt)
{
   IStation* station{static_cast<IStation*>(getParent())};
   station->processTimeCriticalTasks(dt);
   return 0;
}

}
}
