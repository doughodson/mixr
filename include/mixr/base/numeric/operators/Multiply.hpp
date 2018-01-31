
#ifndef __mixr_base_numeric_Multiply_H__
#define __mixr_base_numeric_Multiply_H__

#include "mixr/base/numeric/operators/Operator.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Multiply
// Description: Class for multiplying Numbers
//
// Factory name: "*"
//
//        ( * val n2 ) gives val multiplied by n2.
//
//
// Public method :
//
//      virtual void operation()
//          Performs the numeric operation of the class ( * ).
//------------------------------------------------------------------------------
class Multiply final: public Operator
{
    DECLARE_SUBCLASS(Multiply, Operator)

public:
    Multiply();

private:
    void operation() final;
};

}
}

#endif
