
#ifndef __mixr_base_TeraHertz_HPP__
#define __mixr_base_TeraHertz_HPP__

#include "mixr/base/qty/frequency/IFrequency.hpp"

#include "mixr/base/qty/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  TeraHertz
// Description: Concrete class to specify a frequency in TeraHertz
//------------------------------------------------------------------------------
class TeraHertz final: public IFrequency
{
    DECLARE_SUBCLASS(TeraHertz, IFrequency)

public:
    explicit TeraHertz();

private:
   // convert TeraHertz to Hertz (our base unit)
   double convertToBaseUnit(const double x) final     { return x * frequency::THz2Hz; }
};

}
}

#endif
