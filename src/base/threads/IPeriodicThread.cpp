
#include "mixr/base/threads/IPeriodicThread.hpp"

#include "mixr/base/IComponent.hpp"
#include <iostream>

namespace mixr {
namespace base {

IPeriodicThread::IPeriodicThread(IComponent* const p, const double rt) : IThread(p), rate(rt)
{
}

double IPeriodicThread::getRate() const
{
   return rate;
}

const Statistic& IPeriodicThread::getBustedFrameStats() const
{
   return bfStats;
}

int IPeriodicThread::getTotalFrameCount() const
{
   return tcnt;
}

bool IPeriodicThread::isVariableDeltaTimeEnabled() const
{
   return vdtFlg;
}

bool IPeriodicThread::setVariableDeltaTimeFlag(const bool enable)
{
   vdtFlg = enable;
   return true;
}

}
}
