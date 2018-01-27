
#include "mixr/base/units/frequency/GigaHertz.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(GigaHertz, "GigaHertz")
EMPTY_SLOTTABLE(GigaHertz)
EMPTY_COPYDATA(GigaHertz)
EMPTY_DELETEDATA(GigaHertz)

GigaHertz::GigaHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

GigaHertz::GigaHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

GigaHertz::GigaHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
