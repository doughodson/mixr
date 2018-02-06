
#ifndef __mixr_base_KiloNewtons_H__
#define __mixr_base_KiloNewtons_H__

#include "mixr/base/units/force/Force.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloNewtons
// Description: Concrete class to specify a force in KiloNewtons
//------------------------------------------------------------------------------
class KiloNewtons final: public Force
{
   DECLARE_SUBCLASS(KiloNewtons, Force)

public:
   explicit KiloNewtons();

private:
   // convert KiloNewtons to Newtons (our base unit)
   double convertToBaseUnit(const double x) final     { return x * force::KN2N; }
};

}
}

#endif
