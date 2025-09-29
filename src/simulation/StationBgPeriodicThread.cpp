
#include "StationBgPeriodicThread.hpp"

#include "mixr/simulation/IStation.hpp"

namespace mixr {
namespace simulation {

StationBgPeriodicThread::StationBgPeriodicThread(base::IComponent* const parent, const double rate): base::IPeriodicThread(parent, rate)
{
}

unsigned long StationBgPeriodicThread::userFunc(const double dt)
{
   IStation* station{static_cast<IStation*>(getParent())};
   station->processBackgroundTasks(dt);
   return 0;
}

}
}
