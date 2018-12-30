
#ifndef __mixr_base_numeric_Subtract_HPP__
#define __mixr_base_numeric_Subtract_HPP__

#include "mixr/base/numeric/operators/Operator.hpp"

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
class Subtract final: public Operator
{
   DECLARE_SUBCLASS(Subtract, Operator)

public:
   Subtract();

private:
   // performs the numeric equivalent operation of a "-"
   void operation() final;
   double getValue() const final       { return val; }

   double val{};
};

}
}

#endif
