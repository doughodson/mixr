
#ifndef __mixr_base_Days_H__
#define __mixr_base_Days_H__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Days
// Description: Seconds * 3600.0 * 24.0
//------------------------------------------------------------------------------
class Days final: public Time
{
    DECLARE_SUBCLASS(Days, Time)

public:
    Days();
    Days(const double);
    Days(const Time&);

    static double convertStatic(const Time &n)     { return n.toTime() * time::S2D; }

    double toTime() const final                    { return getValue() * time::D2S; }
    double fromTime(const double a) const final    { return a * time::S2D; }
};

}
}

#endif
