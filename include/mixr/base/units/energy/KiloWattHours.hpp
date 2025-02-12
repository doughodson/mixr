
#ifndef __mixr_KiloWattHours_HPP__
#define __mixr_KiloWattHours_HPP__

#include "mixr/base/units/energy/IEnergy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloWattHours
// Description: Concrete class to specify energy in units of KiloWatt hours
//------------------------------------------------------------------------------
class KiloWattHours final: public IEnergy
{
   DECLARE_SUBCLASS(KiloWattHours, IEnergy)

public:
   explicit KiloWattHours();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final    { return x  * energy::KWH2J; }
};

}
}

#endif
