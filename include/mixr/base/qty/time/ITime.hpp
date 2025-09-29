
#ifndef __mixr_base_Time_HPP__
#define __mixr_base_Time_HPP__

#include "mixr/base/qty/IQuantity.hpp"

#include "mixr/base/qty/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: ITime
// Description: Interface class for time.  Base unit is seconds.
//------------------------------------------------------------------------------
class ITime : public IQuantity
{
   DECLARE_SUBCLASS(ITime, IQuantity)

public:
   explicit ITime();

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
