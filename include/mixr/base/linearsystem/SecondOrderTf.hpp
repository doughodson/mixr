
#ifndef __mixr_linearsystem_SecondOrderTf_HPP__
#define __mixr_linearsystem_SecondOrderTf_HPP__

#include "mixr/base/linearsystem/IDiffEquation.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SecondOrderTf
// ##### IN-WORK #####
//
// Description: Models a second order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coefficients.
//
// Transfer function:
//       N1*s*s + N2*s + N3
//       ------------------
//       D1*s*s + D2*s + D3
//
//------------------------------------------------------------------------------
class SecondOrderTf : public IDiffEquation
{
   DECLARE_SUBCLASS(SecondOrderTf, IDiffEquation)

public:
   SecondOrderTf();
   SecondOrderTf(const unsigned int rate,
      const double n1, const double n2, const double n3,
      const double d1, const double d2, const double d3);

   // Get Parameters
   double getN1() const          { return n1; }
   double getN2() const          { return n2; }
   double getN3() const          { return n3; }
   double getD1() const          { return d1; }
   double getD2() const          { return d2; }
   double getD3() const          { return d3; }

   // Set Parameters
   virtual bool setN1(const double);
   virtual bool setN2(const double);
   virtual bool setN3(const double);
   virtual bool setD1(const double);
   virtual bool setD2(const double);
   virtual bool setD3(const double);

   bool isValid() const override;

protected:
   void initialize() override;

private:
   void initData();

   static const unsigned int ORDER{6};

   // Parameters and initial values
   double n1 {};
   double n2 {};
   double n3 {};
   double d1 {};
   double d2 {};
   double d3 {};
};

}
}

#endif
