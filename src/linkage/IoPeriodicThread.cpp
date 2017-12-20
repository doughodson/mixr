
#include "IoPeriodicThread.hpp"

#include "mixr/linkage/IoHandler.hpp"

#include "mixr/base/Component.hpp"

namespace mixr {
namespace linkage {

IoPeriodicThread::IoPeriodicThread(base::Component* const parent, const double rate): PeriodicThread(parent, rate)
{
}

unsigned long IoPeriodicThread::userFunc(const double dt)
{
   const auto ioHandler = static_cast<IoHandler*>(getParent());
   ioHandler->inputDevices(dt);
   ioHandler->outputDevices(dt);
   return 0;
}

}
}
