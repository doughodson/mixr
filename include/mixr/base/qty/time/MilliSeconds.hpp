
#ifndef __mixr_base_MilliSeconds_HPP__
#define __mixr_base_MilliSeconds_HPP__

#include "mixr/base/qty/time/ITime.hpp"

#include "mixr/base/qty/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MilliSeconds
// Description: Concrete class to specify a time in milliseconds
//------------------------------------------------------------------------------
class MilliSeconds : public ITime
{
   DECLARE_SUBCLASS(MilliSeconds, ITime)

public:
   explicit MilliSeconds();


private:
   // convert milliseconds to seconds (our base unit)
   double convertToBaseUnit(const double x) final     { return x * time::MS2S; }
};

}
}

#endif
