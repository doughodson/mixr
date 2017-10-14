
#include "mixr/base/functors/Polynomial.hpp"

#include "mixr/base/List.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Polynomial, "Polynomial")

BEGIN_SLOTTABLE(Polynomial)
    "coefficients"   // 1) Constant coefficients vector: [ a0 a1 a2 ... aN ]
END_SLOTTABLE(Polynomial)

BEGIN_SLOT_MAP(Polynomial)
    ON_SLOT( 1, setSlotCoefficients, List)
END_SLOT_MAP()

Polynomial::Polynomial()
{
   STANDARD_CONSTRUCTOR()
}

void Polynomial::copyData(const Polynomial& org, const bool)
{
   BaseClass::copyData(org);

   setCoefficients(org.a, org.m);
}

void Polynomial::deleteData()
{
   setCoefficients(nullptr, 0);
}

//------------------------------------------------------------------------------
// The polynomial function
//------------------------------------------------------------------------------
double Polynomial::f(const double x, FStorage* const) const
{
   double result {};
   if (m > 0) {
      const int n {m-1};
      double xx {1.0};
      for (int i = 0; i <= n; i++) {
         result += (a[i] * xx);
         xx *= x;
      }
   }
   return result;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool Polynomial::setCoefficients(const double* const aa, const int mm)
{
   bool ok {};

   // Clear the coefficients
   if (aa == nullptr || mm == 0) {
      for (int i = 0; i < mm; i++) {
         a[i] = 0;
      }
      m = 0;
      ok = true;
   }

   // Copy the coefficients
   else if (mm <= MAX_COEFF) {
      for (int i = 0; i < mm; i++) {
         a[i] = aa[i];
      }
      m = mm;
      ok = true;
   }

   else if (isMessageEnabled(MSG_ERROR)) {
      std::cerr << "Polynomial::setSlotCoefficients(): ERROR; too many coefficients; max is " << (MAX_DEGREE+1) << std::endl;
   }

   return ok;
}

bool Polynomial::setSlotCoefficients(const List* const msg)
{
   bool ok {};

   if (msg != nullptr) {
      unsigned int entries {msg->entries()};
      if ( entries <= MAX_COEFF ) {
         double  aa[MAX_COEFF] {};   // Constant coefficients vector
         unsigned int mm {msg->getNumberList(aa, MAX_COEFF)};
         ok = setCoefficients( aa, mm );
      }

      else if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Polynomial::setSlotCoefficients(): ERROR; too many coefficients; max is " << MAX_COEFF << std::endl;
      }

   }
   else {
      // Just remove the old ...
      ok = setCoefficients(nullptr, 0);
   }

   return ok;
}

}
}
