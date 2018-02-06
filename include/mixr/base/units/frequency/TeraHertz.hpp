
#ifndef __mixr_base_TeraHertz_H__
#define __mixr_base_TeraHertz_H__

#include "mixr/base/units/frequency/Frequency.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  TeraHertz
// Description: Concrete class to specify a frequency in TeraHertz
//------------------------------------------------------------------------------
class TeraHertz final: public Frequency
{
    DECLARE_SUBCLASS(TeraHertz, Frequency)

public:
    explicit TeraHertz();

private:
   // convert TeraHertz to Hertz (our base unit)
   double convertToBaseUnit(const double x) final     { return x * frequency::THz2Hz; }
};

}
}

#endif
