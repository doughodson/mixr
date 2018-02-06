
#ifndef __mixr_base_MegaHertz_H__
#define __mixr_base_MegaHertz_H__

#include "mixr/base/units/frequency/Frequency.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MegaHertz
// Description: Concrete class to specify a frequency in MegaHertz
//------------------------------------------------------------------------------
class MegaHertz final: public Frequency
{
    DECLARE_SUBCLASS(MegaHertz, Frequency)

public:
    explicit MegaHertz();

private:
   // convert MegaHertz to Hertz (our base unit)
   double convertToBaseUnit(const double x) final     { return x * frequency::MHz2Hz; }
};

}
}

#endif
