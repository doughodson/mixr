
#include "mixr/base/functors/Func1.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/functors/Table1.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Func1, "Func1")
EMPTY_SLOTTABLE(Func1)
EMPTY_CONSTRUCTOR(Func1)
EMPTY_COPYDATA(Func1)
EMPTY_DELETEDATA(Func1)

double Func1::f(const double iv1, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table1*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), s);
   }

   return value;
}

bool Func1::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);
   } else if ( msg->isClassType(typeid(Table1)) ) {
      ok = BaseClass::setSlotLfiTable(msg); // We have a 1-D table.
   } else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func1::setSlotLfiTable(): ERROR -- table must use a Table1 or derived class" << std::endl;
   }
   return ok;
}

}
}
