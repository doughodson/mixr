
#include "StationBgPeriodicThread.hpp"

#include "mixr/simulation/Station.hpp"

namespace mixr {
namespace simulation {

StationBgPeriodicThread::StationBgPeriodicThread(base::Component* const parent, const double rate): base::PeriodicThread(parent, rate)
{
}

unsigned long StationBgPeriodicThread::userFunc(const double dt)
{
   Station* station{static_cast<Station*>(getParent())};
   station->processBackgroundTasks(dt);
   return 0;
}

}
}
