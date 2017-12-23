
#ifndef __mixr_base_Func2_H__
#define __mixr_base_Func2_H__

#include "mixr/base/Object.hpp"
#include "mixr/base/functors/Function.hpp"

namespace mixr {
namespace base {
class FStorage;
class Table;

//------------------------------------------------------------------------------
// Class: Func2
// Description: Generic 2-Dimensional function; f(iv1, iv2)
// Factory name: Func2
//------------------------------------------------------------------------------
class Func2 : public Function
{
   DECLARE_SUBCLASS(Func2, Function)
public:
   Func2();

   virtual double f(const double iv1, const double iv2, FStorage* const s = nullptr) const;

protected:
   // slot table helper methods
   bool setSlotLfiTable(const Table* const) override;
};

}
}

#endif
