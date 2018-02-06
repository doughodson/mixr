
#ifndef __mixr_base_StatuteMiles_H__
#define __mixr_base_StatuteMiles_H__

#include "mixr/base/units/length/Length.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: StatuteMiles
// Description: Concrete class to specify length in terms of statute miles
//------------------------------------------------------------------------------
class StatuteMiles final: public Length
{
   DECLARE_SUBCLASS(StatuteMiles, Length)

public:
   explicit StatuteMiles();

private:
   // convert statute miles to meter (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::SM2M; }
};

}
}

#endif
