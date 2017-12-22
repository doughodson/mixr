
#ifndef __mixr_linearsystem_LagFilter_H__
#define __mixr_linearsystem_LagFilter_H__

#include "mixr/linearsystem/FirstOrderTf.hpp"

namespace mixr {
namespace base { class Time; }
namespace linearsystem {

//------------------------------------------------------------------------------
// Class:  LagFilter
//
// Description: Models a first order lag filter with a time constant 'tau' seconds
//
// Transfer function:
//
//           1
//       ---------
//        tau*S + 1
//
// Note: tau must be greater than zero.
//
// Factory name: LagFilter
// Slots:
//    tau    <Time>     Filer time constant
//    tau    <Number>   Filer time constant (sec)
//
//
//------------------------------------------------------------------------------
class LagFilter : public FirstOrderTf
{
   DECLARE_SUBCLASS(LagFilter, FirstOrderTf)

public:
   LagFilter();
   LagFilter(const unsigned int rate, const double tau);

   double getTau() const                         { return tau; }
   virtual bool setTau(const double);

private:
   double tau{1.0};    // filter time constant (seconds)

private:
   // slot table helper methods
   bool setSlotTau(const base::Time* const);
   bool setSlotTau(const base::Number* const);
};

}
}

#endif
