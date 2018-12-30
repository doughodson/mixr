
#ifndef __mixr_base_Hertz_HPP__
#define __mixr_base_Hertz_HPP__

#include "mixr/base/units/frequency/Frequency.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Hertz
// Description: Concrete class to specify a frequency in Hertz
//------------------------------------------------------------------------------
class Hertz final: public Frequency
{
    DECLARE_SUBCLASS(Hertz, Frequency)

public:
    explicit Hertz();

private:
   // convert Hertz to Hertz (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
