
#ifndef __mixr_BTUs_HPP__
#define __mixr_BTUs_HPP__

#include "mixr/base/qty/energy/IEnergy.hpp"

#include "mixr/base/qty/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: BTUs
// Description: Concrete class to specify energy in qty of BTU
//------------------------------------------------------------------------------
class BTUs final: public IEnergy
{
   DECLARE_SUBCLASS(BTUs, IEnergy)

public:
   explicit BTUs();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final    { return x * energy::BTU2J; }
};

}
}

#endif
