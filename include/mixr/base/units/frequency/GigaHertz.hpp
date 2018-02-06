
#ifndef __mixr_base_GigaHertz_H__
#define __mixr_base_GigaHertz_H__

#include "mixr/base/units/frequency/Frequency.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: GigaHertz
// Description: Concrete class to specify a frequency in GigaHertz
//------------------------------------------------------------------------------
class GigaHertz final: public Frequency
{
    DECLARE_SUBCLASS(GigaHertz, Frequency)

public:
    explicit GigaHertz();

private:
   // convert GigaHertz to Hz (our base unit)
   double convertToBaseUnit(const double x) final        { return x * frequency::GHz2Hz; }
};

}
}

#endif
