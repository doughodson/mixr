
#ifndef __mixr_base_Degrees_H__
#define __mixr_base_Degrees_H__

#include "mixr/base/units/angle/Angle.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Degrees
// Description: Concrete class to specify an angle in unit degrees
//------------------------------------------------------------------------------
class Degrees final: public Angle
{
   DECLARE_SUBCLASS(Degrees, Angle)

public:
   explicit Degrees();
   explicit Degrees(const double);

   // temp method to eventually be replaced by setValue() in Quantity class
   void set(const double x)       { setValue(convertToBaseUnit(x)); }

private:
   // convert degrees to radians (our base unit)
   double convertToBaseUnit(const double x) final           { return x * angle::D2RCC; }
};

}
}

#endif

