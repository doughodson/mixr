
#ifndef __mixr_base_Func3_HPP__
#define __mixr_base_Func3_HPP__

#include "mixr/base/Object.hpp"
#include "mixr/base/relations/Function.hpp"

namespace mixr {
namespace base {
class FStorage;
class Table;

//------------------------------------------------------------------------------
// Class: Func3
// Description: Generic 3-Dimensional function; f(iv1, iv2, iv3)
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Func3
// Slots: none
//------------------------------------------------------------------------------
class Func3 : public Function
{
   DECLARE_SUBCLASS(Func3, Function)
public:
   Func3();

   virtual double f(const double iv1, const double iv2, const double iv3, FStorage* const s = nullptr) const;

protected:
   // slot table helper methods
   bool setSlotLfiTable(const Table* const) override;
};

}
}

#endif
