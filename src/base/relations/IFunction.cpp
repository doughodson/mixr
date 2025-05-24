
#include "mixr/base/relations/IFunction.hpp"

#include "mixr/base/relations/ITable.hpp"
#include "mixr/base/relations/IFStorage.hpp"
#include "mixr/base/IList.hpp"

#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(IFunction, "IFunction")

BEGIN_SLOTTABLE(IFunction)
    "table",          // Optional LFI table
END_SLOTTABLE(IFunction)

BEGIN_SLOT_MAP(IFunction)
    ON_SLOT( 1, setSlotLfiTable, ITable)
END_SLOT_MAP()

IFunction::IFunction()
{
   STANDARD_CONSTRUCTOR()

   table = nullptr;
}

void IFunction::copyData(const IFunction& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) table = nullptr;

   {
      const ITable* copy{};
      if (org.table != nullptr) {
         copy = static_cast<const ITable*>(org.table->clone());
      }
      setSlotLfiTable(copy);
      if (copy != nullptr) copy->unref();
   }
}

void IFunction::deleteData()
{
   setSlotLfiTable(nullptr);
}

//------------------------------------------------------------------------------
// Storage factory
//------------------------------------------------------------------------------
IFStorage* IFunction::storageFactory() const
{
   // Since no derived class handled this ...
   if (table != nullptr) {
      // then we're going to let the table handle it, if any
      return table->storageFactory();
   } else {
      // or we're just going to return a standard IFStorage object.
      return new IFStorage();
   }
}

bool IFunction::setSlotLfiTable(const ITable* const x)
{
   table = x;
   return true;
}

}
}
