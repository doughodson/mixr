
#ifndef __mixr_base_Func1_H__
#define __mixr_base_Func1_H__

#include "mixr/base/Object.hpp"
#include "mixr/base/functors/Function.hpp"

namespace mixr {
namespace base {
class FStorage;
class Table;

//------------------------------------------------------------------------------
// Class: Func1
// Description: Generic 1-Dimensional function; f(iv1)
// Factory name: Func1
//------------------------------------------------------------------------------
class Func1 : public Function
{
   DECLARE_SUBCLASS(Func1, Function)
public:
   Func1();

   virtual double f(const double iv1, FStorage* const s = nullptr) const;

protected:
   // slot table helper methods
   bool setSlotLfiTable(const Table* const) override;
};

}
}

#endif
