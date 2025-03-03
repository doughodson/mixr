
#ifndef __mixr_base_Func2_HPP__
#define __mixr_base_Func2_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/relations/IFunction.hpp"

namespace mixr {
namespace base {
class FStorage;
class Table;

//------------------------------------------------------------------------------
// Class: Func2
// Description: Generic 2-Dimensional function; f(iv1, iv2)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Func2
// Slots: none
//------------------------------------------------------------------------------
class Func2 : public IFunction
{
   DECLARE_SUBCLASS(Func2, IFunction)
public:
   Func2();

   virtual double f(const double iv1, const double iv2, FStorage* const s = nullptr) const;

protected:
   // slot table helper methods
   bool setSlotLfiTable(const ITable* const) override;
};

}
}

#endif
