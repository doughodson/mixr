
#include "mixr/base/units/distance/Inches.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Inches, "Inches")
EMPTY_SLOTTABLE(Inches)
EMPTY_COPYDATA(Inches)
EMPTY_DELETEDATA(Inches)

Inches::Inches() : Distance()
{
    STANDARD_CONSTRUCTOR()
}

Inches::Inches(const double value) : Distance(value)
{
    STANDARD_CONSTRUCTOR()
}

Inches::Inches(const Distance& value) : Distance()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
