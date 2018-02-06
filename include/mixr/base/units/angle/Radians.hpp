
#ifndef __mixr_base_Radians_H__
#define __mixr_base_Radians_H__

#include "mixr/base/units/angle/Angle.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Radians
// Description: Concrete class to specify an angle in unit radians
//------------------------------------------------------------------------------
class Radians final: public Angle
{
   DECLARE_SUBCLASS(Radians, Angle)

public:
   explicit Radians();
   explicit Radians(const double);

private:
   // convert radians to radians (our base unit)
   double convertToBaseUnit(const double x) final      { return x; }
};

}
}

#endif

