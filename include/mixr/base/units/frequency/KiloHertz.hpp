
#ifndef __mixr_base_KiloHertz_HPP__
#define __mixr_base_KiloHertz_HPP__

#include "mixr/base/units/frequency/Frequency.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloHertz
// Description: Concrete class to specify a frequency in KiloHertz
//------------------------------------------------------------------------------
class KiloHertz final: public Frequency
{
    DECLARE_SUBCLASS(KiloHertz, Frequency)

public:
    explicit KiloHertz();

private:
   // convert KiloHertz to Hertz (our base unit)
   double convertToBaseUnit(const double x) final     { return x * frequency::KHz2Hz; }
};

}
}

#endif
