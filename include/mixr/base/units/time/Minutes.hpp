
#ifndef __mixr_base_Minutes_H__
#define __mixr_base_Minutes_H__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Minutes
// Description: Seconds * 60.0
//------------------------------------------------------------------------------
class Minutes final: public Time
{
    DECLARE_SUBCLASS(Minutes, Time)

public:
    Minutes();
    Minutes(const double);
    Minutes(const Time&);

    static double convertStatic(const Time &n)      { return n.toTime() * time::S2M; }

    double toTime() const final                     { return getValue() * time::M2S; }
    double fromTime(const double a) const final     { return a * time::S2M; }
};

}
}

#endif
