
#include "mixr/base/units/Unit.hpp"

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Unit, "AbstractUnit")
EMPTY_DELETEDATA(Unit)

BEGIN_SLOTTABLE(Unit)
    "value",
END_SLOTTABLE(Unit)

BEGIN_SLOT_MAP(Unit)
    ON_SLOT(1, setSlotValue, Number)
END_SLOT_MAP()

Unit::Unit()
{
   STANDARD_CONSTRUCTOR()
}

Unit::Unit(const double x) : val(x)
{
   STANDARD_CONSTRUCTOR()
}

void Unit::copyData(const Unit& org, const bool)
{
   BaseClass::copyData(org);
   val = org.val;
}

bool Unit::setSlotValue(const Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      setValue( msg->to_double() );
      ok = true;
   }
   return ok;
}

}
}
