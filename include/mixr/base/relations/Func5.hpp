
#ifndef __mixr_base_Func5_HPP__
#define __mixr_base_Func5_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/relations/IFunction.hpp"

namespace mixr {
namespace base {
class IFStorage;
class ITable;

//------------------------------------------------------------------------------
// Class: Func5
// Description: Generic 5-Dimensional function; f(iv1, iv2, iv3, iv4, iv5)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory names: Func5
// Slots: none
//------------------------------------------------------------------------------
class Func5 : public IFunction
{
   DECLARE_SUBCLASS(Func5, IFunction)
public:
   Func5();

   virtual double f(const double iv1, const double iv2, const double iv3, const double iv4, const double iv5, IFStorage* const s = nullptr) const;

protected:
   // slot table helper methods
   bool setSlotLfiTable(const ITable* const) override;
};

}
}

#endif
