
#ifndef __mixr_base_Semicircles_HPP__
#define __mixr_base_Semicircles_HPP__

#include "mixr/base/qty/angle/IAngle.hpp"

#include "mixr/base/qty/util/angle_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Semicircles
// Description: Concrete class to specify an angle in unit semicircles
//------------------------------------------------------------------------------
class Semicircles final: public IAngle
{
   DECLARE_SUBCLASS(Semicircles, IAngle)

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
