
#ifndef __mixr_base_Hertz_HPP__
#define __mixr_base_Hertz_HPP__

#include "mixr/base/qty/frequency/IFrequency.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Hertz
// Description: Concrete class to specify a frequency in Hertz
//------------------------------------------------------------------------------
class Hertz final: public IFrequency
{
    DECLARE_SUBCLASS(Hertz, IFrequency)

public:
    explicit Hertz();

private:
   // convert Hertz to Hertz (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
