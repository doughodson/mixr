
#ifndef __mixr_base_Func1_HPP__
#define __mixr_base_Func1_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/relations/IFunction.hpp"

namespace mixr {
namespace base {
class FStorage;
class ITable;

//------------------------------------------------------------------------------
// Class: Func1
// Description: Generic 1-Dimensional function; f(iv1)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Func1
// Slots: none
//------------------------------------------------------------------------------
class Func1 : public IFunction
{
   DECLARE_SUBCLASS(Func1, IFunction)
public:
   Func1();

   virtual double f(const double iv1, FStorage* const s = nullptr) const;

protected:
   // slot table helper methods
   bool setSlotLfiTable(const ITable* const) override;
};

}
}

#endif
