
#ifndef __mixr_base_numeric_Add_H__
#define __mixr_base_numeric_Add_H__

#include "mixr/base/numeric/operators/Operator.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Add
// Description: Class for adding Numbers
//------------------------------------------------------------------------------
// Factory name: "+"
//------------------------------------------------------------------------------
// Notes:
//        ( + val n2 ) gives val plus n2
//------------------------------------------------------------------------------
class Add final: public Operator
{
   DECLARE_SUBCLASS(Add, Operator)

public:
   Add();

private:
   // performs the numeric equivalent operation of a "+"
   void operation() final;
   double getValue() const final       { return val; }

   double val{};
};

}
}

#endif
