
#ifndef __mixr_base_Time_H__
#define __mixr_base_Time_H__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Time
// Description: Abstract class for time.  Base unit is seconds.
//------------------------------------------------------------------------------
class Time : public Quantity
{
   DECLARE_SUBCLASS(Time, Quantity)

public:
   explicit Time();

   // conversion methods
   double getValueInDays() const          { return getValue() * time::S2D;  }
   double getValueInHours() const         { return getValue() * time::S2H;  }
   double getValueInMicroSeconds() const  { return getValue() * time::S2US; }
   double getValueInMilliSeconds() const  { return getValue() * time::S2MS; }
   double getValueInMinutes() const       { return getValue() * time::S2M;  }
   double getValueInNanoSeconds() const   { return getValue() * time::S2NS; }
   double getValueInSeconds() const       { return getValue();              }
};

}
}

#endif
