
#include "mixr/simulation/StationNetThread.hpp"

#include "mixr/simulation/Station.hpp"

namespace mixr {
namespace simulation {

NetThread::NetThread(base::Component* const parent, const double priority, const double rate): base::PeriodicTask(parent, priority, rate)
{
}

unsigned long NetThread::userFunc(const double dt)
{
   Station* station {static_cast<Station*>(getParent())};
   station->processNetworkInputTasks(dt);
   station->processNetworkOutputTasks(dt);
   return 0;
}

}
}
