
#include "mixr/base/numeric/operators/IOperator.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(IOperator, "IOperator")
EMPTY_DELETEDATA(IOperator)

BEGIN_SLOTTABLE(IOperator)
    "n2",       //  2nd number (first number is from Number)
    "n3",       //  3rd number
    "n4",       //  4th number
    "n5",       //  5th number
    "n6",       //  6th number
    "n7",       //  7th number
    "n8",       //  8th number
    "n9",       //  9th number
    "n10",      // 10th number
END_SLOTTABLE(IOperator)

BEGIN_SLOT_MAP(IOperator)
    // Accept an Number at any slot index ...
    const auto _msg = dynamic_cast<INumber*>(obj);
    if (_msg != nullptr) { _ok = setSecondNumber(_msg); }
END_SLOT_MAP()

IOperator::IOperator()
{
   STANDARD_CONSTRUCTOR()
}

void IOperator::copyData(const IOperator& org, const bool)
{
   BaseClass::copyData(org);
   n2 = org.n2;
}

bool IOperator::setSecondNumber(const INumber* const x)
{
    n2 = x->asDouble();
    operation();
    return true;
}

}
}
