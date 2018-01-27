
#include "mixr/base/numeric/operators/Add.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Add, "+")
EMPTY_DELETEDATA(Add)

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

void Add::copyData(const Add& org, const bool)
{
   BaseClass::copyData(org);
   n2 = org.n2;
}

void Add::operation()
{
    val = val + n2;
}

bool Add::setSecondNumber(const Number* const ssnobj)
{
    if (ssnobj != nullptr) {
        n2 = ssnobj->to_double();
        operation();
    }
    return true;
}

}
}
