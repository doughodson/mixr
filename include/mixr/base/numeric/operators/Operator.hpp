
#ifndef __mixr_base_numeric_Operator_H__
#define __mixr_base_numeric_Operator_H__

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Operator
// Description: Abstract class to support operations on Numbers
//
// Factory name: "AbstractOperator"
// Slots:
//    n2  <Number>  ! 2nd number (first number is from Number) (default: 0.0)
//    n3  <Number>  ! 3rd number
//    n4  <Number>  ! 4th number
//    n5  <Number>  ! 5th number
//    n6  <Number>  ! 6th number
//    n7  <Number>  ! 7th number
//    n8  <Number>  ! 8th number
//    n9  <Number>  ! 9th number
//    n10 <Number>  ! 10th number
//------------------------------------------------------------------------------
class Operator : public Number
{
    DECLARE_SUBCLASS(Operator, Number)

public:
    Operator();

    double getSecondNumber() const              { return n2; }
    bool setSecondNumber(const Number* const);

protected:
    double n2{};

private:
    virtual void operation() =0;
};

}
}

#endif
