
#include "mixr/base/numeric/Operators.hpp"

#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Add, "+")
EMPTY_DELETEDATA(Add)

IMPLEMENT_SUBCLASS(Subtract, "-")
EMPTY_SLOTTABLE(Subtract)
EMPTY_COPYDATA(Subtract)
EMPTY_DELETEDATA(Subtract)

IMPLEMENT_SUBCLASS(Multiply, "*")
EMPTY_SLOTTABLE(Multiply)
EMPTY_COPYDATA(Multiply)
EMPTY_DELETEDATA(Multiply)

IMPLEMENT_SUBCLASS(Divide, "/")
EMPTY_SLOTTABLE(Divide)
EMPTY_COPYDATA(Divide)
EMPTY_DELETEDATA(Divide)

BEGIN_SLOTTABLE(Add)
    "n2",       //  2nd number (first number is from Number)
    "n3",       //  3rd number
    "n4",       //  4th number
    "n5",       //  5th number
    "n6",       //  6th number
    "n7",       //  7th number
    "n8",       //  8th number
    "n9",       //  9th number
    "n10",      // 10th number
END_SLOTTABLE(Add)

BEGIN_SLOT_MAP(Add)
    // Accept an Number at any slot index ...
    const auto _msg = dynamic_cast<Number*>(obj);
    if (_msg != nullptr) { _ok = setSecondNumber(_msg); }
END_SLOT_MAP()

Add::Add()
{
   STANDARD_CONSTRUCTOR()
}

Subtract::Subtract()
{
   STANDARD_CONSTRUCTOR()
}

Multiply::Multiply()
{
   STANDARD_CONSTRUCTOR()
}

Divide::Divide()
{
   STANDARD_CONSTRUCTOR()
}

void Add::copyData(const Add& org, const bool)
{
   BaseClass::copyData(org);
   n2 = org.n2;
}

//------------------------------------------------------------------------------
// operation() -- the operations
//------------------------------------------------------------------------------
void Add::operation()
{
    val = val + n2;
}

void Subtract::operation()
{
    val = val - n2;
}

void Multiply::operation()
{
    val = val * n2;
}

void Divide::operation()
{
    if (n2 != 0.0) val = (val / n2);  // only if non-zero divisor
}

//------------------------------------------------------------------------------
// setSecondNumber() --
//------------------------------------------------------------------------------
bool Add::setSecondNumber(const Number* const ssnobj)
{
    if (ssnobj != nullptr) {
        n2 = ssnobj->getReal();
        operation();
    }
    return true;
}

}
}
