
#ifndef __mixr_base_numeric_Subtract_H__
#define __mixr_base_numeric_Subtract_H__

#include "mixr/base/numeric/operators/Operator.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Subtract
// Description: Class for subtracting Numbers
//
// Factory name: "-"
//
//        ( - val n2 ) gives val minus n2.
//
//
// Public method :
//
//      virtual void operation()
//          Performs the numeric operation of the class ( - ).
//------------------------------------------------------------------------------
class Subtract final: public Operator
{
    DECLARE_SUBCLASS(Subtract, Operator)

public:
    Subtract();

private:
    void operation() final;
};

}
}

#endif
