
#ifndef __mixr_base_numeric_Add_HPP__
#define __mixr_base_numeric_Add_HPP__

#include "mixr/base/numeric/operators/IOperator.hpp"
#include "mixr/base/util/math_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Add
// Description: Class for adding Numbers
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: "+"
// Slots: none
// Example:
//        ( + val n2 ) gives val plus n2
//------------------------------------------------------------------------------
class Add final: public IOperator
{
   DECLARE_SUBCLASS(Add, IOperator)

public:
   Add();

private:
   // performs the numeric equivalent operation of a "+"
   void operation() final;
   double getValue() const final       { return val; }
   int getIntValue() const final       { return nint(val); }

   double val{};
};

}
}

#endif
