
#include "mixr/base/units/mass/Grams.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Grams, "Grams")
EMPTY_SLOTTABLE(Grams)
EMPTY_COPYDATA(Grams)
EMPTY_DELETEDATA(Grams)

Grams::Grams() : Mass()
{
    STANDARD_CONSTRUCTOR()
}

Grams::Grams(const double value) : Mass(value)
{
    STANDARD_CONSTRUCTOR()
}

Grams::Grams(const Mass& value) : Mass()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value, true);
}

}
}
