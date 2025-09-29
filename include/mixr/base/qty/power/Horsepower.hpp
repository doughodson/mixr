
#ifndef __mixr_base_Horsepower_HPP__
#define __mixr_base_Horsepower_HPP__

#include "mixr/base/qty/power/IPower.hpp"

#include "mixr/base/qty/util/power_utils.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: Horsepower
// Description: Concrete class to specify power in Horsepower
//----------------------------------------------------------------------------
class Horsepower final: public IPower
{
   DECLARE_SUBCLASS(Horsepower, IPower)

public:
   explicit Horsepower();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final     { return x * power::HP2W; }
};

}
}

#endif
