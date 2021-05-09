
#ifndef __mixr_base_numeric_Divide_HPP__
#define __mixr_base_numeric_Divide_HPP__

#include "mixr/base/numeric/operators/Operator.hpp"
#include "mixr/base/util/math_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Divide
// Description: Class for dividing Numbers
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: "/"
// Slots: none
// Example:
//        ( / val n2 ) gives val divided by n2
//------------------------------------------------------------------------------
class Divide final: public Operator
{
   DECLARE_SUBCLASS(Divide, Operator)

public:
   Divide();

private:
   // performs the numeric equivalent operation of a "/"
   // a zero divisor is ignored and produces nothing
   void operation() final;
   double getValue() const final       { return val; }
   int getIntValue() const final       { return nint(val); }

   double val{};
};

}
}

#endif
