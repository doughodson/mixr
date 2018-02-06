
#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Quantity, "AbstractQuantity")
EMPTY_DELETEDATA(Quantity)
EMPTY_CONSTRUCTOR(Quantity)

BEGIN_SLOTTABLE(Quantity)
    "value",
END_SLOTTABLE(Quantity)

BEGIN_SLOT_MAP(Quantity)
    ON_SLOT(1, setSlotValue, Number)
END_SLOT_MAP()

void Quantity::copyData(const Quantity& org, const bool)
{
   BaseClass::copyData(org);
   value = org.value;
}

bool Quantity::setSlotValue(const Number* const x)
{
   bool ok{};
   if (x != nullptr) {
      value = convertToBaseUnit(x->asDouble());
      ok = true;
   }
   return ok;
}

}
}
