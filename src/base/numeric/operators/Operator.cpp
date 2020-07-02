
#include "mixr/base/numeric/operators/Operator.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Operator, "AbstractOperator")
EMPTY_DELETEDATA(Operator)

BEGIN_SLOTTABLE(Operator)
    "n2",       //  2nd number (first number is from Number)
    "n3",       //  3rd number
    "n4",       //  4th number
    "n5",       //  5th number
    "n6",       //  6th number
    "n7",       //  7th number
    "n8",       //  8th number
    "n9",       //  9th number
    "n10",      // 10th number
END_SLOTTABLE(Operator)

BEGIN_SLOT_MAP(Operator)
    // Accept an Number at any slot index ...
    const auto _msg = dynamic_cast<Number*>(obj);
    if (_msg != nullptr) { _ok = setSecondNumber(_msg); }
END_SLOT_MAP()

Operator::Operator()
{
   STANDARD_CONSTRUCTOR()
}

void Operator::copyData(const Operator& org, const bool)
{
   BaseClass::copyData(org);
   n2 = org.n2;
}

bool Operator::setSecondNumber(const Number* const x)
{
    n2 = x->asDouble();
    operation();
    return true;
}

}
}
