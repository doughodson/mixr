
#ifndef __mixr_FootPounds_H__
#define __mixr_FootPounds_H__

#include "mixr/base/units/energy/Energy.hpp"

#include "mixr/base/units/util/energy_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: FootPounds
// Description: Concrete class to specify energy in units of foot pounds
//------------------------------------------------------------------------------
class FootPounds final: public Energy
{
   DECLARE_SUBCLASS(FootPounds, Energy)

public:
   explicit FootPounds();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final     { return x * energy::FP2J;; }
};

}
}

#endif
