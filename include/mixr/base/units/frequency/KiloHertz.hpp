
#ifndef __mixr_base_KiloHertz_H__
#define __mixr_base_KiloHertz_H__

#include "mixr/base/units/frequency/Frequency.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloHertz
// Description:  Hertz * 0.001
//------------------------------------------------------------------------------
class KiloHertz final: public Frequency
{
    DECLARE_SUBCLASS(KiloHertz, Frequency)

public:
    KiloHertz();
    KiloHertz(const double);
    KiloHertz(const Frequency&);

private:
    static double convertStatic(const Frequency &n)      { return n.toFrequency() * frequency::Hz2KHz; }

    double toFrequency() const final                     { return getValue() * frequency::KHz2Hz; }
    double fromFrequency(const double a) const final     { return a * frequency::Hz2KHz; }
};

}
}

#endif
