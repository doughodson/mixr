
#include "mixr/base/numeric/Complex.hpp"

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Complex, "Complex")
EMPTY_DELETEDATA(Complex)

BEGIN_SLOTTABLE(Complex)
   "real",      // 1: real component
   "imag",      // 2: imaginary component
END_SLOTTABLE(Complex)

BEGIN_SLOT_MAP(Complex)
   ON_SLOT(1, setSlotReal,      Number)
   ON_SLOT(2, setSlotImaginary, Number)
END_SLOT_MAP()

Complex::Complex()
{
   STANDARD_CONSTRUCTOR()
}

Complex::Complex(const double r): value(r)
{
   STANDARD_CONSTRUCTOR()
}

Complex::Complex(const double r, const double i) : value(r, i)
{
   STANDARD_CONSTRUCTOR()
}

void Complex::copyData(const Complex& org, const bool)
{
   BaseClass::copyData(org);
   value = org.value;
}

bool Complex::setSlotReal(const Number* const msg)
{
    value.real(msg->asDouble());
    return true;
}

bool Complex::setSlotImaginary(const Number* const msg)
{
    value.imag(msg->asDouble());
    return true;
}

}
}

