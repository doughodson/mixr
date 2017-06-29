
#include "mixr/simulation/StationNetThread.hpp"

#include "mixr/simulation/Station.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_SUBCLASS(NetThread,"NetThread")
EMPTY_SLOTTABLE(NetThread)
EMPTY_COPYDATA(NetThread)
EMPTY_DELETEDATA(NetThread)
EMPTY_SERIALIZER(NetThread)

NetThread::NetThread(base::Component* const parent, const double priority, const double rate): base::PeriodicTask(parent, priority, rate)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long NetThread::userFunc(const double dt)
{
   Station* station = static_cast<Station*>(getParent());
   station->processNetworkInputTasks(dt);
   station->processNetworkOutputTasks(dt);
   return 0;
}

}
}
