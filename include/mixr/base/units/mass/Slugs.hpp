
#ifndef __mixr_base_Slugs_HPP__
#define __mixr_base_Slugs_HPP__

#include "mixr/base/units/mass/IMass.hpp"

#include "mixr/base/units/util/mass_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Slugs
// Description: Concrete class to specify a mass in Slugs
//------------------------------------------------------------------------------
class Slugs : public IMass
{
   DECLARE_SUBCLASS(Slugs, IMass)

public:
   explicit Slugs();

private:
   // convert Slugs to KiloGrams (our base unit)
   double convertToBaseUnit(const double x) final     { return x * mass::SL2KG; }
};

}
}

#endif
