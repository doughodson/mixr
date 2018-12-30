
#ifndef __mixr_BTUs_HPP__
#define __mixr_BTUs_HPP__

#include "mixr/base/units/energy/Energy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: BTUs
// Description: Concrete class to specify energy in units of BTU
//------------------------------------------------------------------------------
class BTUs final: public Energy
{
   DECLARE_SUBCLASS(BTUs, Energy)

public:
   explicit BTUs();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final    { return x * energy::BTU2J; }
};

}
}

#endif
