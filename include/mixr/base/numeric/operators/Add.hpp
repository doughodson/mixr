
#ifndef __mixr_base_numeric_Add_H__
#define __mixr_base_numeric_Add_H__

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Add
// Description:  Class for adding Numbers.
//
// Factory name: "+"
// Slots:
//    n2  <Number>  ! 2nd number (first number is from Number) (default: 0.0)
//    n3  <Number>  ! 3rd number
//    n4  <Number>  ! 4th number
//    n5  <Number>  ! 5th number
//    n6  <Number>  ! 6th number
//    n7  <Number>  ! 7th number
//    n8  <Number>  ! 8th number
//    n9  <Number>  ! 9th number
//    n10  <Number> ! 10th number
//
//        ( + val n2 ) gives val plus n2.
//
//
// Public method :
//
//      virtual void operation()
//          Performs the numeric operation of the class ( + ).
//------------------------------------------------------------------------------
class Add : public Number
{
    DECLARE_SUBCLASS(Add, Number)

public:
    Add();

    double getSecondNumber() const              { return n2; }

    virtual bool setSecondNumber(const Number* const);
    virtual void operation();

protected:
    double n2{};
};

}
}

#endif
