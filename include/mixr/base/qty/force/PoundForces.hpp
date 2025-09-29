
#ifndef __mixr_base_PoundForces_HPP__
#define __mixr_base_PoundForces_HPP__

#include "mixr/base/qty/force/IForce.hpp"

#include "mixr/base/qty/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: PoundForces
// Description: Concrete class to specify a force in Pound Forces
//------------------------------------------------------------------------------
class PoundForces final: public IForce
{
   DECLARE_SUBCLASS(PoundForces, IForce)

public:
   explicit PoundForces();

private:
   // convert pound force to newtons (our base unit)
   double convertToBaseUnit(const double x) final      { return x * force::PF2N; }
};

}
}

#endif
