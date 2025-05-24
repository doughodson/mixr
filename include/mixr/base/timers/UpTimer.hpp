
#ifndef __mixr_base_UpTimer_HPP__
#define __mixr_base_UpTimer_HPP__

#include "mixr/base/timers/ITimer.hpp"

namespace mixr {
namespace base {
class Boolean;
class ITime;

//------------------------------------------------------------------------------
// Class: UpTimer
// Description: Concrete UP timer; time counts in the positive direction.
//------------------------------------------------------------------------------
// Factory name: UpTimer
//------------------------------------------------------------------------------
class UpTimer : public ITimer
{
    DECLARE_SUBCLASS(UpTimer, ITimer)

public:
    UpTimer(const double rtime = 0.0);
};

}
}

#endif
