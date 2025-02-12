
#ifndef __mixr_base_Grams_HPP__
#define __mixr_base_Grams_HPP__

#include "mixr/base/units/mass/IMass.hpp"

#include "mixr/base/units/util/mass_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Grams
// Description: Concrete class to specify a mass in Grams
//------------------------------------------------------------------------------
class Grams : public IMass
{
   DECLARE_SUBCLASS(Grams, IMass)

public:
   explicit Grams();

private:
   // convert Grams to KiloGrams (our base unit)
   double convertToBaseUnit(const double x) final     { return x * mass::G2KG; }
};

}
}

#endif
