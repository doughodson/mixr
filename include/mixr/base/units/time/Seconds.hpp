
#ifndef __mixr_base_Seconds_H__
#define __mixr_base_Seconds_H__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Seconds
// Description: An instance of Seconds with its value equal to 1.0 is one base unit for times.
//------------------------------------------------------------------------------
class Seconds final: public Time
{
    DECLARE_SUBCLASS(Seconds, Time)

public:
    Seconds();
    Seconds(const double);
    Seconds(const Time&);

    static double convertStatic(const Time &n)      { return n.toTime(); }

    double toTime() const final                     { return getValue(); }
    double fromTime(const double a) const final     { return a; }
};

}
}

#endif
