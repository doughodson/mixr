
#ifndef __mixr_linearsystem_IFirstOrderTf_HPP__
#define __mixr_linearsystem_IFirstOrderTf_HPP__

#include "mixr/base/linearsystem/IDiffEquation.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  IFirstOrderTf
// Base class:  base::IObject -> IScalerFunc-> IDiffEquation -> IFirstOrderTf
//
// Description: Interface to continuous transfer functions in the discrete
//              z-domain using a difference equation with Tustin coefficients.
//
// Transfer function:
//       N1*s + N2
//       ---------
//       D1*s + D2
//
//------------------------------------------------------------------------------
class IFirstOrderTf : public IDiffEquation
{
   DECLARE_SUBCLASS(IFirstOrderTf, IDiffEquation)

public:
   IFirstOrderTf();
   IFirstOrderTf(const unsigned int rate, const double n1, const double n2, const double d1, const double d2);

   // Get Parameters
   double getN1() const          { return n1; }
   double getN2() const          { return n2; }
   double getD1() const          { return d1; }
   double getD2() const          { return d2; }

   // Set Parameters
   virtual bool setN1(const double v);
   virtual bool setN2(const double v);
   virtual bool setD1(const double v);
   virtual bool setD2(const double v);

   bool isValid() const override;

protected:
   void initialize() override;

private:
   void initData();  // Initialize our data

   static const unsigned int ORDER{2};

   // Parameters and initial values
   double n1 {};
   double n2 {};
   double d1 {};
   double d2 {};
};

}
}

#endif
