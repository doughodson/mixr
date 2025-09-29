
#ifndef __mixr_base_MegaHertz_HPP__
#define __mixr_base_MegaHertz_HPP__

#include "mixr/base/qty/frequency/IFrequency.hpp"

#include "mixr/base/qty/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MegaHertz
// Description: Concrete class to specify a frequency in MegaHertz
//------------------------------------------------------------------------------
class MegaHertz final: public IFrequency
{
    DECLARE_SUBCLASS(MegaHertz, IFrequency)

public:
    explicit MegaHertz();

private:
   // convert MegaHertz to Hertz (our base unit)
   double convertToBaseUnit(const double x) final     { return x * frequency::MHz2Hz; }
};

}
}

#endif
