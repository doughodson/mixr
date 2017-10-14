
#include "mixr/base/functors/Func2.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/functors/Table2.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Func2, "Func2")
EMPTY_SLOTTABLE(Func2)
EMPTY_CONSTRUCTOR(Func2)
EMPTY_COPYDATA(Func2)
EMPTY_DELETEDATA(Func2)

double Func2::f(const double iv1, const double iv2, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table2*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), static_cast<double>(iv2), s);
   }

   return value;
}

bool Func2::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);
   } else if ( msg->isClassType(typeid(Table2)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 2-D table.
   } else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func2::setSlotLfiTable(): ERROR -- table must use a Table2 or derived class" << std::endl;
   }
   return ok;
}

}
}
