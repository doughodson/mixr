
#ifndef __mixr_base_numeric_Add_H__
#define __mixr_base_numeric_Add_H__

#include "mixr/base/numeric/operators/Operator.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Add
// Description: Class for adding Numbers.
//
// Factory name: "+"
//
//        ( + val n2 ) gives val plus n2
//
//
// Public method :
//
//      void operation()
//          Performs the numeric operation of the class ( + )
//------------------------------------------------------------------------------
class Add final: public Operator
{
    DECLARE_SUBCLASS(Add, Operator)

public:
    Add();

private:
    void operation() final;
};

}
}

#endif
