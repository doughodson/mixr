
#include "mixr/base/numeric/Float.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Float, "Float")
EMPTY_COPYDATA(Float)
EMPTY_DELETEDATA(Float)

BEGIN_SLOTTABLE(Float)
    "value",
END_SLOTTABLE(Float)

BEGIN_SLOT_MAP(Float)
    ON_SLOT(1, setSlotValue, Float)
END_SLOT_MAP()

bool Float::setSlotValue(const Float* const num)
{
   bool ok{};
   if (num != nullptr) {
      val = static_cast<double>(num->asDouble());
      ok = true;
   }
   return ok;
}

}
}
