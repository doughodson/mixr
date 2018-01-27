
#include "mixr/base/units/volume/Liters.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Liters, "Liters")
EMPTY_SLOTTABLE(Liters)
EMPTY_COPYDATA(Liters)
EMPTY_DELETEDATA(Liters)

Liters::Liters() : Volume()
{
    STANDARD_CONSTRUCTOR()
}

Liters::Liters(const double value) : Volume(value)
{
    STANDARD_CONSTRUCTOR()
}
Liters::Liters(const Volume& value) : Volume()
{
    STANDARD_CONSTRUCTOR()
    BaseClass::copyData(value,true);
}

}
}
