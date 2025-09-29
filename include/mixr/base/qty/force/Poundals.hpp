
#ifndef __mixr_base_Poundals_HPP__
#define __mixr_base_Poundals_HPP__

#include "mixr/base/qty/force/IForce.hpp"

#include "mixr/base/qty/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Poundals
// Description: Concrete class to specify a force in Poundals
//------------------------------------------------------------------------------
class Poundals final: public IForce
{
   DECLARE_SUBCLASS(Poundals, IForce)

public:
   explicit Poundals();

private:
   // convert poundals to newtons (our base unit)
   double convertToBaseUnit(const double x) final     { return x * force::PD2N; }
};

}
}

#endif
