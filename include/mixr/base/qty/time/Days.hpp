
#ifndef __mixr_base_Days_HPP__
#define __mixr_base_Days_HPP__

#include "mixr/base/qty/time/ITime.hpp"

#include "mixr/base/qty/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Days
// Description: Concrete class to specify a time in days
//------------------------------------------------------------------------------
class Days final: public ITime
{
   DECLARE_SUBCLASS(Days, ITime)

public:
   explicit Days();

private:
   // convert days to seconds (our base unit)
   double convertToBaseUnit(const double x) final     { return x * time::D2S; }
};

}
}

#endif
