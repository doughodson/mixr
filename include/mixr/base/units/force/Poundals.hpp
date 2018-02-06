
#ifndef __mixr_base_Poundals_H__
#define __mixr_base_Poundals_H__

#include "mixr/base/units/force/Force.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Poundals
// Description: Concrete class to specify a force in Poundals
//------------------------------------------------------------------------------
class Poundals final: public Force
{
   DECLARE_SUBCLASS(Poundals, Force)

public:
   explicit Poundals();

private:
   // convert poundals to newtons (our base unit)
   double convertToBaseUnit(const double x) final     { return x * force::PD2N; }
};

}
}

#endif
