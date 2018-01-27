
#include "mixr/base/units/mass/KiloGrams.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(KiloGrams, "KiloGrams")
EMPTY_SLOTTABLE(KiloGrams)
EMPTY_COPYDATA(KiloGrams)
EMPTY_DELETEDATA(KiloGrams)

KiloGrams::KiloGrams() : Mass()
{
    STANDARD_CONSTRUCTOR()
}

KiloGrams::KiloGrams(const double value) : Mass(value)
{
    STANDARD_CONSTRUCTOR()
}

KiloGrams::KiloGrams(const Mass& value) : Mass()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}
