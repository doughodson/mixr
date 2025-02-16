
#ifndef __mixr_linearsystem_LowpassFilter_HPP__
#define __mixr_linearsystem_LowpassFilter_HPP__

#include "mixr/base/linearsystem/FirstOrderTf.hpp"

namespace mixr {
namespace base {
class IFrequency;
class INumber;

//------------------------------------------------------------------------------
// Class: LowpassFilter
//
// Description: Models a first order low pass filter with cutoff frequency of Wc radians/sec
//
// Transfer function:
//
//           wc
//       ---------
//         s + wc
//
// Factory name: Lag1Filter
// Slots:
//    wc    <Frequency>  Cutoff Frequency
//    wc    <Number>     Cutoff frequency (radian/sec)
//
//------------------------------------------------------------------------------
class LowpassFilter : public FirstOrderTf
{
   DECLARE_SUBCLASS(LowpassFilter, FirstOrderTf)

public:
   LowpassFilter();
   LowpassFilter(const unsigned int rate, const double w);

   double getWc() const                           { return wc; }
   virtual bool setWc(const double);

private:
   double wc{};    // Cutoff frequency (rad/sec)

private:
   // slot table helper methods
   bool setSlotWc(const base::IFrequency* const);
   bool setSlotWc(const base::INumber* const);
};

}
}

#endif
