
#include "mixr/base/numeric/Integer.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Integer, "Integer")
EMPTY_COPYDATA(Integer)
EMPTY_DELETEDATA(Integer)

BEGIN_SLOTTABLE(Integer)
    "value",
END_SLOTTABLE(Integer)

BEGIN_SLOT_MAP(Integer)
    ON_SLOT(1, setSlotValue, Integer)
END_SLOT_MAP()

bool Integer::setSlotValue(const Integer* const num)
{
   bool ok{};
   if (num != nullptr) {
      val = num->asInt();
      ok = true;
   }
   return ok;
}

}
}
