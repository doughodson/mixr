
#ifndef __mixr_base_KiloNewtons_HPP__
#define __mixr_base_KiloNewtons_HPP__

#include "mixr/base/units/force/IForce.hpp"

#include "mixr/base/units/util/force_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloNewtons
// Description: Concrete class to specify a force in KiloNewtons
//------------------------------------------------------------------------------
class KiloNewtons final: public IForce
{
   DECLARE_SUBCLASS(KiloNewtons, IForce)

public:
   explicit KiloNewtons();

private:
   // convert KiloNewtons to Newtons (our base unit)
   double convertToBaseUnit(const double x) final     { return x * force::KN2N; }
};

}
}

#endif
