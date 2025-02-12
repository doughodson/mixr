
#ifndef __mixr_FootPounds_HPP__
#define __mixr_FootPounds_HPP__

#include "mixr/base/units/energy/IEnergy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: FootPounds
// Description: Concrete class to specify energy in units of foot pounds
//------------------------------------------------------------------------------
class FootPounds final: public IEnergy
{
   DECLARE_SUBCLASS(FootPounds, IEnergy)

public:
   explicit FootPounds();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final     { return x * energy::FP2J;; }
};

}
}

#endif
