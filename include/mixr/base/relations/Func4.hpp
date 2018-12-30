
#ifndef __mixr_base_Func4_HPP__
#define __mixr_base_Func4_HPP__

#include "mixr/base/Object.hpp"
#include "mixr/base/relations/Function.hpp"

namespace mixr {
namespace base {
class FStorage;
class Table;

//------------------------------------------------------------------------------
// Class: Func4
// Description: Generic 4-Dimensional function; f(iv1, iv2, iv3, iv4)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Func4
// Slots: none
//------------------------------------------------------------------------------
class Func4 : public Function
{
   DECLARE_SUBCLASS(Func4, Function)
public:
   Func4();

   virtual double f(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const s = nullptr) const;

protected:
   // slot table helper methods
   bool setSlotLfiTable(const Table* const) override;
};

}
}

#endif
