
#ifndef __mixr_Energy_HPP__
#define __mixr_Energy_HPP__

#include "mixr/base/units/IQuantity.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: IEnergy
// Description: Interface class for power. Base unit is a Joule.
//------------------------------------------------------------------------------
class IEnergy : public IQuantity
{
   DECLARE_SUBCLASS(IEnergy, IQuantity)

public:
   explicit IEnergy();

   // conversion methods
   double getValueInBTUs() const            { return getValue() * energy::BTU2J; }
   double getValueInCalories() const        { return getValue() * energy::C2J;   }
   double getValueInFootPoinds() const      { return getValue() * energy::FP2J;  }
   double getValueInJoules() const          { return getValue();                 }
   double getValueInKiloWattHours() const   { return getValue() * energy::KWH2J; }
};

}
}

#endif
