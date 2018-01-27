
#ifndef __mixr_base_GigaHertz_H__
#define __mixr_base_GigaHertz_H__

#include "mixr/base/units/frequency/Frequency.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: GigaHertz
// Description:  Hertz * 0.000000001
//------------------------------------------------------------------------------
class GigaHertz final: public Frequency
{
    DECLARE_SUBCLASS(GigaHertz, Frequency)

public:
    GigaHertz();
    GigaHertz(const double);
    GigaHertz(const Frequency&);

    static double convertStatic(const Frequency &n)      { return n.toFrequency() * frequency::Hz2GHz; }

    double toFrequency() const final                     { return getValue() * frequency::GHz2Hz; }
    double fromFrequency(const double a) const final     { return a * frequency::Hz2GHz; }
};

}
}

#endif
