
#ifndef __mixr_base_numeric_Complex_HPP__
#define __mixr_base_numeric_Complex_HPP__

#include "mixr/base/IObject.hpp"

#include <complex>

namespace mixr {
namespace base {
class INumber;

//------------------------------------------------------------------------------
// Class: Complex
// Description: Generic complex number class
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Complex
// Slots:
//     real  <Number>  ! real value (default: 0.0)
//     imag  <Number>  ! imaginary value (default: 0.0)
//------------------------------------------------------------------------------
class Complex final: public IObject
{
   DECLARE_SUBCLASS(Complex, IObject)

public:
   explicit Complex();
   explicit Complex(const double r);
   explicit Complex(const double r, const double i);

   void setValue(const std::complex<double>& x)     { value = x;    }
   const std::complex<double>& asComplex() const    { return value; }

   void setReal(const double x)                     { value.real(x);       }
   double getReal() const                           { return value.real(); }

   void setImag(const double x)                     { value.imag(x);       }
   double getImag() const                           { return value.imag(); }

private:
   std::complex<double> value{};

private:
   // slot table helper methods
   bool setSlotReal(const INumber* const);
   bool setSlotImaginary(const INumber* const);
};

}
}

#endif
