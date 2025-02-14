
#ifndef __mixr_base_numeric_Multiply_HPP__
#define __mixr_base_numeric_Multiply_HPP__

#include "mixr/base/numeric/operators/IOperator.hpp"
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
class Multiply final: public IOperator
{
   DECLARE_SUBCLASS(Multiply, IOperator)

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
