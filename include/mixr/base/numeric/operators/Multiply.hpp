
#ifndef __mixr_base_numeric_Multiply_HPP__
#define __mixr_base_numeric_Multiply_HPP__

#include "mixr/base/numeric/operators/Operator.hpp"
#include "mixr/base/util/math_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Multiply
// Description: Class for multiplying Numbers
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: "*"
// Slots: none
// Example:
//        ( * val n2 ) gives val multiplied by n2
//------------------------------------------------------------------------------
class Multiply final: public Operator
{
   DECLARE_SUBCLASS(Multiply, Operator)

public:
   Multiply();

private:
   // performs the numeric equivalent operation of a "*"
   void operation() final;
   double getValue() const final       { return val; }
   int getIntValue() const final       { return nint(val); }

   double val{};
};

}
}

#endif
