
#ifndef __mixr_base_NauticalMiles_HPP__
#define __mixr_base_NauticalMiles_HPP__

#include "mixr/base/units/length/Length.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: NauticalMiles
// Description: Concrete class to specify length in terms of nautical miles
//------------------------------------------------------------------------------
class NauticalMiles final: public Length
{
   DECLARE_SUBCLASS(NauticalMiles, Length)

public:
   explicit NauticalMiles();

private:
   // convert nautical miles to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::NM2M; }
};

}
}

#endif
