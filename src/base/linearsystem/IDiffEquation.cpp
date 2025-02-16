
#include "mixr/base/linearsystem/IDiffEquation.hpp"
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(IDiffEquation, "IDiffEquation")
EMPTY_SLOTTABLE(IDiffEquation)
EMPTY_DELETEDATA(IDiffEquation)

IDiffEquation::IDiffEquation()
{
   STANDARD_CONSTRUCTOR()
}

IDiffEquation::IDiffEquation(const unsigned int r) : IScalerFunc(r)
{
   STANDARD_CONSTRUCTOR()
}

void IDiffEquation::copyData(const IDiffEquation& org, const bool)
{
   BaseClass::copyData(org);

   // copy data array -- derived classes must have called allocateMemory() prior
   // to calling this copyData() function.
   allocateMemory( org.n );
   for (unsigned int i{}; i < n; i++) {
      pa[i] = org.pa[i];
      pb[i] = org.pb[i];
   }
}

//------------------------------------------------------------------------------
// Allocate memory arrays and free any old arrays
//------------------------------------------------------------------------------
void IDiffEquation::allocateMemory(unsigned int n0)
{
   if (n0 != n) {
      // Free the old memory arrays
      if (n > 0) {
         delete[] pa;  pa = nullptr;
         delete[] pb;  pb = nullptr;
      }

      BaseClass::allocateMemory(n0);

      // Allocate the new space
      if (n0 > 0) {
         pa = new double[n0];
         pb = new double[n0];
      }
   }
}

//------------------------------------------------------------------------------
// Clear the memory arrays
//------------------------------------------------------------------------------
void IDiffEquation::clearMemory()
{
   for (unsigned int i{}; i < n; i++) {
      pa[i] = 0;
      pa[i] = 0;
   }
}

//------------------------------------------------------------------------------
// g() is one iteration of the difference equation.
//------------------------------------------------------------------------------
double IDiffEquation::g(const double xn)
{
   if (isValid()) {

      // Shift (age) the past input and output values
      for (unsigned int k{(n-1)}; k > 0; k--) {
         px[k] = px[k-1];
         py[k] = py[k-1];
      }

      // Current input
      px[0] = xn;

      // The difference equation ... compute the current output
      py[0] = 0;
      for (unsigned int k{1}; k < n; k++) {
         // past outputs
         py[0] += pa[k]*py[k];
      }
      for (unsigned int k{}; k < n; k++) {
         // Inputs
         py[0] += pb[k]*px[k];
      }

      return py[0];
   }
   else {
      // If invalid transfer function, just return the input value
      return xn;
   }
}

}
}
