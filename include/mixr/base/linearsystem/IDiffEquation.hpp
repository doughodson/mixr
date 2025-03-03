
#ifndef __mixr_linearsystem_IDiffEquation_HPP__
#define __mixr_linearsystem_IDiffEquation_HPP__

#include "mixr/base/linearsystem/IScalerFunc.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  IDiffEquation
//
// Description: Interface for discrete z-domain transfer functions.  Core
//              engine is a difference equation that has its coefficients initialized
//              by the various derived classes.
//
//------------------------------------------------------------------------------
class IDiffEquation : public IScalerFunc
{
    DECLARE_SUBCLASS(IDiffEquation, IScalerFunc)

public:
   IDiffEquation();
   IDiffEquation(const unsigned int rate);

   double g(const double x) override;

protected:
   void allocateMemory(unsigned int n) override;
   void clearMemory() override;

   // Difference equation coef arrays
   double* pa {};    // (a[k] * y[k]) terms; for k = 1 .. n
   double* pb {};    // (b[k] * x[k]) terms; for k = 0 .. n
};

}
}

#endif
