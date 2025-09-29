
#include "mixr/base/qty/IQuantity.hpp"

#include "mixr/base/numeric/INumber.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(IQuantity, "IQuantity")
EMPTY_DELETEDATA(IQuantity)
EMPTY_CONSTRUCTOR(IQuantity)

BEGIN_SLOTTABLE(IQuantity)
    "value",
END_SLOTTABLE(IQuantity)

BEGIN_SLOT_MAP(IQuantity)
    ON_SLOT(1, setSlotValue, INumber)
END_SLOT_MAP()

void IQuantity::copyData(const IQuantity& org, const bool)
{
   BaseClass::copyData(org);
   value = org.value;
}

bool IQuantity::setSlotValue(const INumber* const x)
{
   value = convertToBaseUnit(x->asDouble());
   return true;
}

}
}
