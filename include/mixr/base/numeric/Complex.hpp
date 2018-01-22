
#ifndef __mixr_base_numeric_Complex_H__
#define __mixr_base_numeric_Complex_H__

#include "mixr/base/Object.hpp"

#include <complex>

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Complex
// Description: Generic complex number class
//
// Factory name: Complex
// Slots:
//     real  <Number>  ! real value (default: 0.0)
//     imag  <Number>  ! imaginary value (default: 0.0)
//------------------------------------------------------------------------------
class Complex : public Object
{
   DECLARE_SUBCLASS(Complex, Object)

public:
   Complex();
   Complex(const double r);
   Complex(const double r, const double i);

   void setReal(const double x)             { value.real(x);       }
   double getReal() const                   { return value.real(); }

   void setImag(const double x)             { value.imag(x);       }
   double getImag() const                   { return value.imag(); }

   const std::complex<double>& getComplex() { return value; }

private:
   std::complex<double> value{};

private:
   // slot table helper methods
   bool setSlotReal(const Number* const);
   bool setSlotImaginary(const Number* const);
};

}
}

#endif
