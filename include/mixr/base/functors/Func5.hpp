
#ifndef __mixr_base_Func5_H__
#define __mixr_base_Func5_H__

#include "mixr/base/Object.hpp"
#include "mixr/base/functors/Function.hpp"

namespace mixr {
namespace base {
class FStorage;
class Table;

//------------------------------------------------------------------------------
// Class: Func5
// Description: Generic 5-Dimensional function; f(iv1, iv2, iv3, iv4, iv5)
// Factory names: Func5
//------------------------------------------------------------------------------
class Func5 : public Function
{
   DECLARE_SUBCLASS(Func5, Function)
public:
   Func5();

   virtual double f(const double iv1, const double iv2, const double iv3, const double iv4, const double iv5, FStorage* const s = nullptr) const;

protected:
   // slot table helper methods
   bool setSlotLfiTable(const Table* const) override;
};

}
}

#endif
