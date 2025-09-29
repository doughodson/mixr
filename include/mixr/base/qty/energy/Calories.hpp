
#ifndef __mixr_Calories_HPP__
#define __mixr_Calories_HPP__

#include "mixr/base/qty/energy/IEnergy.hpp"

#include "mixr/base/qty/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Calories
// Description: Concrete class to specify energy in qty of Calories
//------------------------------------------------------------------------------
class Calories final: public IEnergy
{
   DECLARE_SUBCLASS(Calories, IEnergy)

public:
   explicit Calories();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final      { return x * energy::C2J; }
};

}
}

#endif
