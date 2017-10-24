
#include "mixr/base/functors/Func5.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/functors/Table5.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Func5, "Func5")
EMPTY_SLOTTABLE(Func5)
EMPTY_CONSTRUCTOR(Func5)
EMPTY_COPYDATA(Func5)
EMPTY_DELETEDATA(Func5)

double Func5::f(const double iv1, const double iv2, const double iv3, const double iv4, const double iv5, FStorage* const s) const
{
   double value {};

   // No derived class handled this ...

   const auto p = static_cast<const Table5*>(getTable());
   if (p != nullptr) {
      // But we do have an optional table that'll handle it.
      value = p->lfi(static_cast<double>(iv1), static_cast<double>(iv2), static_cast<double>(iv3),
                     static_cast<double>(iv4), static_cast<double>(iv5) ,s);
   }

   return value;
}

bool Func5::setSlotLfiTable(const Table* const msg)
{
   bool ok {};
   if (msg == nullptr) {
      ok = BaseClass::setSlotLfiTable(nullptr);
   } else if ( msg->isClassType(typeid(Table5)) ) {
      ok = BaseClass::setSlotLfiTable(msg);  // We have a 5-D table.
   } else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Func5::setSlotLfiTable(): ERROR -- table must use a Table5 or derived class" << std::endl;
   }
   return ok;
}

}
}
