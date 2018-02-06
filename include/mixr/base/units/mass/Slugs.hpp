
#ifndef __mixr_base_Slugs_H__
#define __mixr_base_Slugs_H__

#include "mixr/base/units/mass/Mass.hpp"

#include "mixr/base/units/util/mass_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Slugs
// Description: Concrete class to specify a mass in Slugs
//------------------------------------------------------------------------------
class Slugs : public Mass
{
   DECLARE_SUBCLASS(Slugs, Mass)

public:
   explicit Slugs();

private:
   // convert Slugs to KiloGrams (our base unit)
   double convertToBaseUnit(const double x) final     { return x * mass::SL2KG; }
};

}
}

#endif
