
#include "IoThread.hpp"

#include "mixr/iolinkage/IoHandler.hpp"

#include "mixr/base/Component.hpp"

namespace mixr {
namespace iolinkage {

IMPLEMENT_SUBCLASS(IoThread, "IoThread")
EMPTY_SLOTTABLE(IoThread)
EMPTY_COPYDATA(IoThread)
EMPTY_DELETEDATA(IoThread)

IoThread::IoThread(base::Component* const parent, const double priority, const double rate): PeriodicTask(parent, priority, rate)
{
   STANDARD_CONSTRUCTOR()
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
