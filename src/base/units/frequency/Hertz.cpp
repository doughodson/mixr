
#include "mixr/base/units/frequency/Hertz.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Hertz, "Hertz")
EMPTY_SLOTTABLE(Hertz)
EMPTY_COPYDATA(Hertz)
EMPTY_DELETEDATA(Hertz)

Hertz::Hertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

Hertz::Hertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

Hertz::Hertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
