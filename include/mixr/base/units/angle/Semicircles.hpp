
#ifndef __mixr_base_Semicircles_HPP__
#define __mixr_base_Semicircles_HPP__

#include "mixr/base/units/angle/Angle.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Semicircles
// Description: Concrete class to specify an angle in unit semicircles
//------------------------------------------------------------------------------
class Semicircles final: public Angle
{
   DECLARE_SUBCLASS(Semicircles, Angle)

public:
   explicit Semicircles();
   explicit Semicircles(const double);

private:
   // convert semicircles to radians (our base unit)
   double convertToBaseUnit(const double x) final      { return x * angle::SC2R; }
};

}
}

#endif
