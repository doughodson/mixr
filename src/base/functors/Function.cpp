
#include "mixr/base/functors/Function.hpp"

#include "mixr/base/functors/Table.hpp"
#include "mixr/base/functors/FStorage.hpp"
#include "mixr/base/List.hpp"

#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Function, "Function")

BEGIN_SLOTTABLE(Function)
    "table",          // Optional LFI table
END_SLOTTABLE(Function)

BEGIN_SLOT_MAP(Function)
    ON_SLOT( 1, setSlotLfiTable, Table)
END_SLOT_MAP()

Function::Function()
{
   STANDARD_CONSTRUCTOR()

   table = nullptr;
}

void Function::copyData(const Function& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) table = nullptr;

   {
      const Table* copy{};
      if (org.table != nullptr) {
         copy = static_cast<const Table*>(org.table->clone());
      }
      setSlotLfiTable(copy);
      if (copy != nullptr) copy->unref();
   }
}

void Function::deleteData()
{
   setSlotLfiTable(nullptr);
}

//------------------------------------------------------------------------------
// Storage factory
//------------------------------------------------------------------------------
FStorage* Function::storageFactory() const
{
   // Since no derived class handled this ...
   if (table != nullptr) {
      // then we're going to let the table handle it, if any
      return table->storageFactory();
   } else {
      // or we're just going to return a standard FStorage object.
      return new FStorage();
   }
}

bool Function::setSlotLfiTable(const Table* const msg)
{
   table = msg;
   return true;
}

}
}
