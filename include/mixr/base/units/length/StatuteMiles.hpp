
#ifndef __mixr_base_StatuteMiles_HPP__
#define __mixr_base_StatuteMiles_HPP__

#include "mixr/base/units/length/ILength.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: StatuteMiles
// Description: Concrete class to specify length in terms of statute miles
//------------------------------------------------------------------------------
class StatuteMiles final: public ILength
{
   DECLARE_SUBCLASS(StatuteMiles, ILength)

public:
   explicit StatuteMiles();

private:
   // convert statute miles to meter (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::SM2M; }
};

}
}

#endif
