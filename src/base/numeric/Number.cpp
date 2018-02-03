
#include "mixr/base/numeric/Number.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Number, "AbstractNumber")
EMPTY_SLOTTABLE(Number)
EMPTY_DELETEDATA(Number)
EMPTY_COPYDATA(Number)

/*
BEGIN_SLOTTABLE(Number)
    "value",
END_SLOTTABLE(Number)

BEGIN_SLOT_MAP(Number)
    ON_SLOT(1, setSlotValue, Number)
END_SLOT_MAP()
*/

/*
bool Number::setSlotValue(const Number* const num)
{
   bool ok{};
   if (num != nullptr) {
      val = num->asDouble();
      ok = true;
   }
   return ok;
}
*/

}
}
