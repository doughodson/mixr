
#include "IoThread.hpp"

#include "mixr/linkage/IoHandler.hpp"

#include "mixr/base/Component.hpp"

namespace mixr {
namespace linkage {

IoThread::IoThread(base::Component* const parent, const double priority, const double rate): PeriodicTask(parent, priority, rate)
{
}

unsigned long IoThread::userFunc(const double dt)
{
   const auto ioHandler = static_cast<IoHandler*>(getParent());
   ioHandler->inputDevices(dt);
   ioHandler->outputDevices(dt);
   return 0;
}

}
}
