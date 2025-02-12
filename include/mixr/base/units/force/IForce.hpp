
#ifndef __mixr_base_IForce_HPP__
#define __mixr_base_IForce_HPP__

#include "mixr/base/units/IQuantity.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: IForce
// Description: Interface class for force.  Base unit is Newton.
//------------------------------------------------------------------------------
class IForce : public IQuantity
{
   DECLARE_SUBCLASS(IForce, IQuantity)

public:
   explicit IForce();

   // conversion methods
   double getValueInKiloNewtons() const     { return getValue() * force::N2KN; }
   double getValueInNewtons() const         { return getValue();               }
   double getValueInPoundals() const        { return getValue() * force::N2PD; }
   double getValueInPoundForces() const     { return getValue() * force::N2PF; }
};

}
}

#endif
