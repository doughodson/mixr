
#ifndef __mixr_base_Power_HPP__
#define __mixr_base_Power_HPP__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/power_utils.hpp"
#include <cmath>

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: Power
// Description: Abstract class for power.  Base unit is Watts.
//----------------------------------------------------------------------------
class Power : public Quantity
{
   DECLARE_SUBCLASS(Power, Quantity)

public:
   explicit Power();

   // conversion methods
   double getValueInDecibelMilliWatts() const  { return 10.0 * std::log10(getValue() * power::W2MW); }
   double getValueInDecibelWatts() const       { return 10.0 * std::log10(getValue());               }
   double getValueInHorsepower() const         { return getValue() * power::W2HP;                    }
   double getValueInKiloWatts() const          { return getValue() * power::W2KW;                    }
   double getValueInMilliWatts() const         { return getValue() * power::W2MW;                    }
   double getValueInWatts() const              { return getValue();                                  }
};

}
}

#endif
