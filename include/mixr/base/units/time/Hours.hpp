
#ifndef __mixr_base_Hours_H__
#define __mixr_base_Hours_H__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Hours
// Description: Seconds * 3600.0
//------------------------------------------------------------------------------
class Hours final: public Time
{
    DECLARE_SUBCLASS(Hours, Time)

public:
    Hours();
    Hours(const double);
    Hours(const Time&);

    static double convertStatic(const Time &n)      { return n.toTime() * time::S2H; }

    double toTime() const final                     { return getValue() * time::H2S; }
    double fromTime(const double a) const final     { return a * time::S2H; }
};

}
}

#endif
