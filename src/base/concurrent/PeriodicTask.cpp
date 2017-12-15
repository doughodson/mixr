
#include "mixr/base/concurrent/PeriodicTask.hpp"

#include "mixr/base/Component.hpp"
#include <iostream>

namespace mixr {
namespace base {

PeriodicTask::PeriodicTask(Component* const p, const double pri, const double rt) : Thread(p, pri), rate(rt)
{
}

//------------------------------------------------------------------------------
// get/set functions
//------------------------------------------------------------------------------
double PeriodicTask::getRate() const
{
   return rate;
}

const Statistic& PeriodicTask::getBustedFrameStats() const
{
   return bfStats;
}

unsigned int PeriodicTask::getTotalFrameCount() const
{
   return tcnt;
}

bool PeriodicTask::isVariableDeltaTimeEnabled() const
{
   return vdtFlg;
}

bool PeriodicTask::setVariableDeltaTimeFlag(const bool enable)
{
   vdtFlg = enable;
   return true;
}

}
}
