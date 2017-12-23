
#ifndef __mixr_base_Polynomial_H__
#define __mixr_base_Polynomial_H__

#include "mixr/base/Object.hpp"
#include "mixr/base/functors/Func1.hpp"

namespace mixr {
namespace base {
class FStorage;
class List;

//------------------------------------------------------------------------------
// Class: Polynomial
// Description: Polynomial function
//                f(x) = a0 + a1*x + a2*x^2 + ... + aN*x^N
//
// Factory names: Polynomial
// Slots:
//    coefficients   <base::List>  ! Constant coefficients vector: [ a0 a1 a2 ... aN ]
//
// Notes
//    1) The degree of the polynomial is determined by the size of the coefficients vector.
//
//          coefficients     degree            result
//          ------------    --------     --------------------------------
//             none            < 0        0
//              1               0         a0
//              2               1         a0 + a1 * x
//              3               2         a0 + a1 * x + a2 * x^2
//              M             N = M-1     a0 + a1 * x + a2 * x^2 + ... + aN * x^N
//
//    2) Storage is not used.
//
//------------------------------------------------------------------------------
class Polynomial : public Func1
{
   DECLARE_SUBCLASS(Polynomial, Func1)

public:
   // Highest allowed degree of polynomial
   static const int MAX_DEGREE{32};

public:
   Polynomial();

   int getDegree() const                  { return (m-1); }
   const double* getCoefficients() const  { return a; }

   double f(const double x, FStorage* const s = nullptr) const override;

protected:
   bool setCoefficients(const double* const coeff, const int n);

private:
   static const int MAX_COEFF{MAX_DEGREE+1};

   double a[MAX_COEFF]{};   // Constant coefficients vector
   int m{};                 // Number of coefficients (degree + 1)

protected:
   // slot table helper methods
   virtual bool setSlotCoefficients(const List* const);
};

}
}

#endif
