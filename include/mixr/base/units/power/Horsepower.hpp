
#ifndef __mixr_base_Horsepower_HPP__
#define __mixr_base_Horsepower_HPP__

#include "mixr/base/units/power/Power.hpp"

#include "mixr/base/units/util/power_utils.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: Horsepower
// Description: Concrete class to specify power in Horsepower
//----------------------------------------------------------------------------
class Horsepower final: public Power
{
   DECLARE_SUBCLASS(Horsepower, Power)

public:
   explicit Horsepower();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final     { return x * power::HP2W; }
};

}
}

#endif
