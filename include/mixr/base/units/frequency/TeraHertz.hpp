
#ifndef __mixr_base_TeraHertz_H__
#define __mixr_base_TeraHertz_H__

#include "mixr/base/units/frequency/Frequency.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  TeraHertz
// Description:  Hertz * 0.000000000001
//------------------------------------------------------------------------------
class TeraHertz final: public Frequency
{
    DECLARE_SUBCLASS(TeraHertz, Frequency)

public:
    TeraHertz();
    TeraHertz(const double);
    TeraHertz(const Frequency&);

    static double convertStatic(const Frequency &n)      { return n.toFrequency() * frequency::Hz2THz; }

    double toFrequency() const final                     { return getValue() * frequency::THz2Hz; }
    double fromFrequency(const double a) const final     { return a * frequency::Hz2THz; }
};

}
}

#endif
