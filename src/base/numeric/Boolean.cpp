
#include "mixr/base/numeric/Boolean.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Boolean, "Boolean")
EMPTY_DELETEDATA(Boolean)

BEGIN_SLOTTABLE(Boolean)
    "value",
END_SLOTTABLE(Boolean)

BEGIN_SLOT_MAP(Boolean)
    ON_SLOT(1, setSlotValue, Boolean)
END_SLOT_MAP()

void Boolean::copyData(const Boolean& org, const bool)
{
   BaseClass::copyData(org);
   value = org.value;
}

bool Boolean::setSlotValue(const Boolean* const x)
{
   value = x->value;
   return true;
}

}
}
