
#ifndef __mixr_base_DownTimer_HPP__
#define __mixr_base_DownTimer_HPP__

#include "mixr/base/timers/ITimer.hpp"

namespace mixr {
namespace base {
class Boolean;
class ITime;

//------------------------------------------------------------------------------
// Class: DownTimer
// Description: Concrete DOWN timer; time counts in the negative direction.
//------------------------------------------------------------------------------
// Factory name: DownTimer
//------------------------------------------------------------------------------
class DownTimer : public ITimer
{
    DECLARE_SUBCLASS(DownTimer, ITimer)

public:
    DownTimer(const double rtime = 0.0);
};

}
}

#endif
