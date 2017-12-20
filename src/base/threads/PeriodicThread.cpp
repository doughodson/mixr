
#include "mixr/base/threads/PeriodicThread.hpp"

#include "mixr/base/Component.hpp"
#include <iostream>

namespace mixr {
namespace base {

PeriodicThread::PeriodicThread(Component* const p, const double rt) : AbstractThread(p), rate(rt)
{
}

double PeriodicThread::getRate() const
{
   return rate;
}

const Statistic& PeriodicThread::getBustedFrameStats() const
{
   return bfStats;
}

int PeriodicThread::getTotalFrameCount() const
{
   return tcnt;
}

bool PeriodicThread::isVariableDeltaTimeEnabled() const
{
   return vdtFlg;
}

bool PeriodicThread::setVariableDeltaTimeFlag(const bool enable)
{
   vdtFlg = enable;
   return true;
}

}
}
