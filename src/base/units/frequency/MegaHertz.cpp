
#include "mixr/base/units/frequency/MegaHertz.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(MegaHertz, "MegaHertz")
EMPTY_SLOTTABLE(MegaHertz)
EMPTY_COPYDATA(MegaHertz)
EMPTY_DELETEDATA(MegaHertz)

MegaHertz::MegaHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

MegaHertz::MegaHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

MegaHertz::MegaHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
