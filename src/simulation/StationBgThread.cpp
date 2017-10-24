
#include "mixr/simulation/StationBgThread.hpp"

#include "mixr/simulation/Station.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_SUBCLASS(BgThread,"BgThread")
EMPTY_SLOTTABLE(BgThread)
EMPTY_COPYDATA(BgThread)
EMPTY_DELETEDATA(BgThread)

BgThread::BgThread(base::Component* const parent, const double priority, const double rate): base::PeriodicTask(parent, priority, rate)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long BgThread::userFunc(const double dt)
{
   Station* station {static_cast<Station*>(getParent())};
   station->processBackgroundTasks(dt);
   return 0;
}

}
}
