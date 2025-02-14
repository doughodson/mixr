
#ifndef __mixr_base_numeric_IOperator_HPP__
#define __mixr_base_numeric_IOperator_HPP__

#include "mixr/base/numeric/INumber.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: IOperator
// Description: Interface to support operations on Numbers
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: IOperator
// Slots:
//    n2  <INumber>  ! 2nd number (first number is from Number) (default: 0.0)
//    n3  <INumber>  ! 3rd number
//    n4  <INumber>  ! 4th number
//    n5  <INumber>  ! 5th number
//    n6  <INumber>  ! 6th number
//    n7  <INumber>  ! 7th number
//    n8  <INumber>  ! 8th number
//    n9  <INumber>  ! 9th number
//    n10 <INumber>  ! 10th number
//------------------------------------------------------------------------------
class IOperator : public INumber
{
    DECLARE_SUBCLASS(IOperator, INumber)

public:
    IOperator();

    double getSecondNumber() const              { return n2; }
    bool setSecondNumber(const INumber* const);

protected:
    double n2{};

private:
    virtual void operation() =0;
};

}
}

#endif
