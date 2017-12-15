
#include "mixr/simulation/StationBgThread.hpp"

#include "mixr/simulation/Station.hpp"

namespace mixr {
namespace simulation {

BgThread::BgThread(base::Component* const parent, const double priority, const double rate): base::PeriodicTask(parent, priority, rate)
{
}

unsigned long BgThread::userFunc(const double dt)
{
   Station* station {static_cast<Station*>(getParent())};
   station->processBackgroundTasks(dt);
   return 0;
}

}
}
