
#include "StationNetPeriodicThread.hpp"

#include "mixr/simulation/IStation.hpp"

namespace mixr {
namespace simulation {

StationNetPeriodicThread::StationNetPeriodicThread(base::IComponent* const parent, const double rate): base::IPeriodicThread(parent, rate)
{
}

unsigned long StationNetPeriodicThread::userFunc(const double dt)
{
   IStation* station{static_cast<IStation*>(getParent())};
   station->processNetworkInputTasks(dt);
   station->processNetworkOutputTasks(dt);
   return 0;
}

}
}
