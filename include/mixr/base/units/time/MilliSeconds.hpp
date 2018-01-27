
#ifndef __mixr_base_MilliSeconds_H__
#define __mixr_base_MilliSeconds_H__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MilliSeconds
// Description: Seconds / 1000.0
//------------------------------------------------------------------------------
class MilliSeconds : public Time
{
    DECLARE_SUBCLASS(MilliSeconds, Time)

public:
    MilliSeconds();
    MilliSeconds(const double);
    MilliSeconds(const Time&);

    static double convertStatic(const Time &n)      { return n.toTime() * time::S2MS; }

    double toTime() const final                     { return getValue() * time::MS2S; }
    double fromTime(const double a) const final     { return a * time::S2MS; }
};

}
}

#endif
