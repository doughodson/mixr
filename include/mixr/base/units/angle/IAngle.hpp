
#ifndef __mixr_base_IAngle_HPP__
#define __mixr_base_IAngle_HPP__

#include "mixr/base/units/IQuantity.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: IAngle
// Description: Interface class for plane angles - base unit is Radians
//------------------------------------------------------------------------------
class IAngle : public IQuantity
{
   DECLARE_SUBCLASS(IAngle, IQuantity)

public:
   explicit IAngle();

   // conversion methods
   double getValueInDegrees() const            { return getValue() * angle::R2DCC; }
   double getValueInRadians() const            { return getValue();                }
   double getValueInSemicircles() const        { return getValue() * angle::R2SC;  }
};

}
}

#endif

