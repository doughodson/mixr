
#ifndef __mixr_base_Hertz_H__
#define __mixr_base_Hertz_H__

#include "mixr/base/units/frequency/Frequency.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Hertz
// Description:  Base unit for frequency, with a hertz being an instance of Hertz
//               with its value equal to 1.0.
//------------------------------------------------------------------------------
class Hertz final: public Frequency
{
    DECLARE_SUBCLASS(Hertz, Frequency)

public:
    Hertz();
    Hertz(const double);
    Hertz(const Frequency&);

    static double convertStatic(const Frequency &n)      { return n.toFrequency(); }

    double toFrequency() const final                     { return getValue(); }
    double fromFrequency(const double a) const final     { return a; }
};

}
}

#endif
