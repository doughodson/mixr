
#ifndef __mixr_base_Volume_HPP__
#define __mixr_base_Volume_HPP__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Volume
// Description: Abstract class for volume.  Base unit is cubic meters (m^3).
//------------------------------------------------------------------------------
class Volume : public Quantity
{
   DECLARE_SUBCLASS(Volume, Quantity)

public:
   explicit Volume();

   // conversion methods
   double getValueInCubicFeet() const      { return getValue() * volume::CM2CFT;  }
   double getValueInCubicInches() const    { return getValue() * volume::CM2CIN;  }
   double getValueInCubicMeters() const    { return getValue();                   }
   double getValueInLiters() const         { return getValue() * volume::CM2L;    }
};

}
}

#endif
