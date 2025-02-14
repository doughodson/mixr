
#include "mixr/base/numeric/Complex.hpp"

#include "mixr/base/numeric/INumber.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Complex, "Complex")
EMPTY_DELETEDATA(Complex)

BEGIN_SLOTTABLE(Complex)
   "real",      // 1: real component
   "imag",      // 2: imaginary component
END_SLOTTABLE(Complex)

BEGIN_SLOT_MAP(Complex)
   ON_SLOT(1, setSlotReal,      INumber)
   ON_SLOT(2, setSlotImaginary, INumber)
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

bool Complex::setSlotReal(const INumber* const x)
{
    value.real(x->asDouble());
    return true;
}

bool Complex::setSlotImaginary(const INumber* const x)
{
    value.imag(x->asDouble());
    return true;
}

}
}

