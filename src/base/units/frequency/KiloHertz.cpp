
#include "mixr/base/units/frequency/KiloHertz.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(KiloHertz, "KiloHertz")
EMPTY_SLOTTABLE(KiloHertz)
EMPTY_COPYDATA(KiloHertz)
EMPTY_DELETEDATA(KiloHertz)

KiloHertz::KiloHertz() : Frequency()
{
    STANDARD_CONSTRUCTOR()
}

KiloHertz::KiloHertz(const double value) : Frequency(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloHertz::KiloHertz(const Frequency& value) : Frequency()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
