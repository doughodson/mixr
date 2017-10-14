
#include "mixr/base/functors/Func3.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/functors/Table3.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Func3, "Func3")
EMPTY_SLOTTABLE(Func3)
EMPTY_CONSTRUCTOR(Func3)
EMPTY_COPYDATA(Func3)
EMPTY_DELETEDATA(Func3)

double Func3::f(const double iv1, const double iv2, const double iv3, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table3*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), static_cast<double>(iv2), static_cast<double>(iv3), s);
   }

   return value;
}

bool Func3::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);
   } else if ( msg->isClassType(typeid(Table3)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 3-D table.
   } else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func3::setSlotLfiTable(): ERROR -- table must use a Table3 or derived class" << std::endl;
   }
   return ok;
}

}
}
