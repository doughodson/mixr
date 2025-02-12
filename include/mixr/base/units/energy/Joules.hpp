
#ifndef __mixr_Joules_HPP__
#define __mixr_Joules_HPP__

#include "mixr/base/units/energy/IEnergy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Joules
// Description: Concrete class to specify energy in units of Joules
//------------------------------------------------------------------------------
class Joules final: public IEnergy
{
   DECLARE_SUBCLASS(Joules, IEnergy)

public:
   explicit Joules();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
