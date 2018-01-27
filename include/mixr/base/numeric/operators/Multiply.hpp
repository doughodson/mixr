
#ifndef __mixr_base_numeric_Multiply_H__
#define __mixr_base_numeric_Multiply_H__

#include "mixr/base/numeric/operators/Add.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  Multiply
// Description:  Class for multiplying Numbers.
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
class Multiply : public Add
{
    DECLARE_SUBCLASS(Multiply, Add)

public:
    Multiply();
    void operation() override;
};

}
}

#endif
