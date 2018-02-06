
#ifndef __mixr_Calories_H__
#define __mixr_Calories_H__

#include "mixr/base/units/energy/Energy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Calories
// Description: Concrete class to specify energy in units of Calories
//------------------------------------------------------------------------------
class Calories final: public Energy
{
   DECLARE_SUBCLASS(Calories, Energy)

public:
   explicit Calories();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final      { return x * energy::C2J; }
};

}
}

#endif
