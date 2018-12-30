
#ifndef __mixr_base_Force_HPP__
#define __mixr_base_Force_HPP__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Force
// Description: Abstract class for force.  Base unit is Newton.
//------------------------------------------------------------------------------
class Force : public Quantity
{
   DECLARE_SUBCLASS(Force, Quantity)

public:
   explicit Force();

   // conversion methods
   double getValueInKiloNewtons() const     { return getValue() * force::N2KN; }
   double getValueInNewtons() const         { return getValue();               }
   double getValueInPoundals() const        { return getValue() * force::N2PD; }
   double getValueInPoundForces() const     { return getValue() * force::N2PF; }
};

}
}

#endif
