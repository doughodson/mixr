
#ifndef __mixr_base_Hours_HPP__
#define __mixr_base_Hours_HPP__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Hours
// Description: Concrete class to specify a time in hours
//------------------------------------------------------------------------------
class Hours final: public Time
{
   DECLARE_SUBCLASS(Hours, Time)

public:
   explicit Hours();

private:
   // convert hours to seconds (our base unit)
   double convertToBaseUnit(const double x) final     { return x * time::H2S; }
};

}
}

#endif
