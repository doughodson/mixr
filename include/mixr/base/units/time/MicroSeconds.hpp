
#ifndef __mixr_base_MicroSeconds_H__
#define __mixr_base_MicroSeconds_H__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MicroSeconds
// Description: Seconds / 1000000.0
//------------------------------------------------------------------------------
class MicroSeconds final: public Time
{
    DECLARE_SUBCLASS(MicroSeconds, Time)

public:
    MicroSeconds();
    MicroSeconds(const double);
    MicroSeconds(const Time&);

    static double convertStatic(const Time &n)      { return n.toTime() * time::S2US; }

    double toTime() const final                     { return getValue() * time::US2S; }
    double fromTime(const double a) const final     { return a * time::S2US; }
};

}
}

#endif
