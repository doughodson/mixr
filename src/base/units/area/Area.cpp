
#include "mixr/base/units/area/Area.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Area, "AbstractArea")
EMPTY_SLOTTABLE(Area)
EMPTY_DELETEDATA(Area)

Area::Area()
{
   STANDARD_CONSTRUCTOR()
}

Area::Area(const double value) : Unit(value) 
{
   STANDARD_CONSTRUCTOR()
}

void Area::copyData(const Area& org, const bool)
{
   BaseClass::copyData(org);
   setValue(fromArea(org.toArea()));
}

}
}
