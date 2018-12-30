
#ifndef __mixr_base_PoundForces_HPP__
#define __mixr_base_PoundForces_HPP__

#include "mixr/base/units/force/Force.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: PoundForces
// Description: Concrete class to specify a force in Pound Forces
//------------------------------------------------------------------------------
class PoundForces final: public Force
{
   DECLARE_SUBCLASS(PoundForces, Force)

public:
   explicit PoundForces();

private:
   // convert pound force to newtons (our base unit)
   double convertToBaseUnit(const double x) final      { return x * force::PF2N; }
};

}
}

#endif
