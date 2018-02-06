
#ifndef __mixr_base_MilliSeconds_H__
#define __mixr_base_MilliSeconds_H__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MilliSeconds
// Description: Concrete class to specify a time in milliseconds
//------------------------------------------------------------------------------
class MilliSeconds : public Time
{
   DECLARE_SUBCLASS(MilliSeconds, Time)

public:
   explicit MilliSeconds();


private:
   // convert milliseconds to seconds (our base unit)
   double convertToBaseUnit(const double x) final     { return x * time::MS2S; }
};

}
}

#endif
