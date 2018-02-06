
#ifndef __mixr_base_Angle_H__
#define __mixr_base_Angle_H__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Angle
// Description: Abstract class for plane angles.  Base unit is Radians.
//------------------------------------------------------------------------------
class Angle : public Quantity
{
   DECLARE_SUBCLASS(Angle, Quantity)

public:
   explicit Angle();

   // conversion methods
   double getValueInDegrees() const            { return getValue() * angle::R2DCC; }
   double getValueInRadians() const            { return getValue();                }
   double getValueInSemicircles() const        { return getValue() * angle::R2SC;  }
};

}
}

#endif

