
#include "mixr/base/functors/Func4.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/functors/Table4.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Func4, "Func4")
EMPTY_SLOTTABLE(Func4)
EMPTY_CONSTRUCTOR(Func4)
EMPTY_COPYDATA(Func4)
EMPTY_DELETEDATA(Func4)

double Func4::f(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table4*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), static_cast<double>(iv2),
                     static_cast<double>(iv3), static_cast<double>(iv4), s);
   }

   return value;
}

bool Func4::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);    // Setting the table to null.
   } else if ( msg->isClassType(typeid(Table4)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 4-D table.
   } else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func4::setSlotLfiTable(): ERROR -- table must use a Table4 or derived class" << std::endl;
   }
   return ok;
}

}
}
