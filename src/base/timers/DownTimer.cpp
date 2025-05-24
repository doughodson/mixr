
#include "mixr/base/timers/DownTimer.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(DownTimer, "DownTimer")
EMPTY_SLOTTABLE(DownTimer)
EMPTY_COPYDATA(DownTimer)
EMPTY_DELETEDATA(DownTimer)

DownTimer::DownTimer(const double rtime) : ITimer(Type::DOWN, rtime)
{
}


}
}

