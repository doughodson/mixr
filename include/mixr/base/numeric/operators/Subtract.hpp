
#ifndef __mixr_base_numeric_Subtract_HPP__
#define __mixr_base_numeric_Subtract_HPP__

#include "mixr/base/numeric/operators/IOperator.hpp"
#include "mixr/base/util/math_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Subtract
// Description: Class for subtracting Numbers
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: "-"
// Slots: none
// Example:
//        ( - val n2 ) gives val minus n2
//------------------------------------------------------------------------------
class Subtract final: public IOperator
{
   DECLARE_SUBCLASS(Subtract, IOperator)

public:
   Subtract();

private:
   // performs the numeric equivalent operation of a "-"
   void operation() final;
   double getValue() const final       { return val; }
   int getIntValue() const final       { return nint(val); }

   double val{};
};

}
}

#endif
