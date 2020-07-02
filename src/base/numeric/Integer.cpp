
#include "mixr/base/numeric/Integer.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Integer, "Integer")
EMPTY_DELETEDATA(Integer)

BEGIN_SLOTTABLE(Integer)
    "value",
END_SLOTTABLE(Integer)

BEGIN_SLOT_MAP(Integer)
    ON_SLOT(1, setSlotValue, Integer)
END_SLOT_MAP()

void Integer::copyData(const Integer& org, const bool)
{
   BaseClass::copyData(org);
   value = org.value;
}

bool Integer::setSlotValue(const Integer* const x)
{
   value = x->asInt();
   return true;
}

}
}
