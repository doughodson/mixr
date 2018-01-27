
#include "mixr/base/units/frequency/TeraHertz.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(TeraHertz, "TeraHertz")
EMPTY_SLOTTABLE(TeraHertz)
EMPTY_COPYDATA(TeraHertz)
EMPTY_DELETEDATA(TeraHertz)

TeraHertz::TeraHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

TeraHertz::TeraHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

TeraHertz::TeraHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
