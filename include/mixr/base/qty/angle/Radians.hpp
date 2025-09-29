
#ifndef __mixr_base_Radians_HPP__
#define __mixr_base_Radians_HPP__

#include "mixr/base/qty/angle/IAngle.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Radians
// Description: Concrete class to specify an angle in unit radians
//------------------------------------------------------------------------------
class Radians final: public IAngle
{
   DECLARE_SUBCLASS(Radians, IAngle)

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

