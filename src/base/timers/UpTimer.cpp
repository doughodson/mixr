
#include "mixr/base/timers/UpTimer.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(UpTimer, "UpTimer")
EMPTY_SLOTTABLE(UpTimer)
EMPTY_COPYDATA(UpTimer)
EMPTY_DELETEDATA(UpTimer)

UpTimer::UpTimer(const double rtime) : ITimer(Type::UP, rtime)
{
}

}
}

