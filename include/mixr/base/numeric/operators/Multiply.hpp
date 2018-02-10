
#ifndef __mixr_base_numeric_Multiply_H__
#define __mixr_base_numeric_Multiply_H__

#include "mixr/base/numeric/operators/Operator.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Multiply
// Description: Class for multiplying Numbers
//------------------------------------------------------------------------------
// Factory name: "*"
//------------------------------------------------------------------------------
// Notes:
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

   double val{};
};

}
}

#endif
