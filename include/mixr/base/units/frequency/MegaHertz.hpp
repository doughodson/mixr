
#ifndef __mixr_base_MegaHertz_H__
#define __mixr_base_MegaHertz_H__

#include "mixr/base/units/frequency/Frequency.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MegaHertz
// Description:  Hertz * 0.000001
//------------------------------------------------------------------------------
class MegaHertz final: public Frequency
{
    DECLARE_SUBCLASS(MegaHertz, Frequency)

public:
    MegaHertz();
    MegaHertz(const double);
    MegaHertz(const Frequency&);

    static double convertStatic(const Frequency &n)      { return n.toFrequency() * frequency::Hz2MHz; }

    double toFrequency() const final                     { return getValue() * frequency::MHz2Hz; }
    double fromFrequency(const double a) const final     { return a * frequency::Hz2MHz; }
};

}
}

#endif
