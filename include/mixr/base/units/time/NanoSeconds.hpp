
#ifndef __mixr_base_NanoSeconds_H__
#define __mixr_base_NanoSeconds_H__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: NanoSeconds
// Description: Seconds / 1000000000.0
//------------------------------------------------------------------------------
class NanoSeconds final: public Time
{
    DECLARE_SUBCLASS(NanoSeconds, Time)

public:
    NanoSeconds();
    NanoSeconds(const double);
    NanoSeconds(const Time&);

    static double convertStatic(const Time &n)      { return n.toTime() * time::S2NS; }

    double toTime() const final                     { return getValue() * time::NS2S; }
    double fromTime(const double a) const final     { return a * time::S2NS; }
};

}
}

#endif
