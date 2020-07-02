
#include "mixr/base/numeric/Float.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Float, "Float")
EMPTY_DELETEDATA(Float)

BEGIN_SLOTTABLE(Float)
    "value",
END_SLOTTABLE(Float)

BEGIN_SLOT_MAP(Float)
    ON_SLOT(1, setSlotValue, Float)
END_SLOT_MAP()

void Float::copyData(const Float& org, const bool)
{
   BaseClass::copyData(org);
   value = org.value;
}

bool Float::setSlotValue(const Float* const x)
{
   value = x->asDouble();
   return true;
}

}
}
