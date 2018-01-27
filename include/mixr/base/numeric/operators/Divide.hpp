
#ifndef __mixr_base_numeric_Divide_H__
#define __mixr_base_numeric_Divide_H__

#include "mixr/base/numeric/operators/Add.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  Divide
// Description:  Class for dividing Numbers.
//
// Factory name: "/"
//
//        ( / val n2 ) gives val divided by n2.
//
//
// Public method :
//
//      virtual void operation()
//          Performs the numeric operation of the class ( / ).
//          Note:  In Divide, a zero divisor causes operation() to do nothing.
//------------------------------------------------------------------------------
class Divide : public Add
{
    DECLARE_SUBCLASS(Divide, Add)

public:
    Divide();

    void operation() override;
};

}
}

#endif
