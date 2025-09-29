
#ifndef __mixr_base_GigaHertz_HPP__
#define __mixr_base_GigaHertz_HPP__

#include "mixr/base/qty/frequency/IFrequency.hpp"

#include "mixr/base/qty/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: GigaHertz
// Description: Concrete class to specify a frequency in GigaHertz
//------------------------------------------------------------------------------
class GigaHertz final: public IFrequency
{
    DECLARE_SUBCLASS(GigaHertz, IFrequency)

public:
    explicit GigaHertz();

private:
   // convert GigaHertz to Hz (our base unit)
   double convertToBaseUnit(const double x) final        { return x * frequency::GHz2Hz; }
};

}
}

#endif
