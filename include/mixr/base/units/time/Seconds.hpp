
#ifndef __mixr_base_Seconds_HPP__
#define __mixr_base_Seconds_HPP__

#include "mixr/base/units/time/Time.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Seconds
// Description: Concrete class to specify a time in seconds
//------------------------------------------------------------------------------
class Seconds final: public Time
{
   DECLARE_SUBCLASS(Seconds, Time)

public:
   explicit Seconds();

private:
   // convert seconds to seconds (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
