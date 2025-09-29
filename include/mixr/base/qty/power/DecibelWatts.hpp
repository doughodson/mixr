
#ifndef __mixr_base_DecibelWatts_HPP__
#define __mixr_base_DecibelWatts_HPP__

#include "mixr/base/qty/power/IPower.hpp"

#include "mixr/base/qty/util/power_utils.hpp"

#include <cmath>

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: DecibelWatts
// Description: Concrete class to specify power in Decibel Watts
//----------------------------------------------------------------------------
class DecibelWatts final: public IPower
{
   DECLARE_SUBCLASS(DecibelWatts, IPower)

public:
   explicit DecibelWatts();

private:
   // convert Decibel Watts to Watts (our base unit)
   double convertToBaseUnit(const double x) final     { return std::pow(10.0, x / 10.0); }
};

}
}

#endif
