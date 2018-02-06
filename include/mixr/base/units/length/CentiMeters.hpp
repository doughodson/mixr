
#ifndef __mixr_base_CentiMeters_H__
#define __mixr_base_CentiMeters_H__

#include "mixr/base/units/length/Length.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CentiMeters
// Description: Concrete class to specify length in terms of centimeters
//------------------------------------------------------------------------------
class CentiMeters final: public Length
{
   DECLARE_SUBCLASS(CentiMeters, Length)

public:
   explicit CentiMeters();

private:
   // convert centimeters to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::CM2M; }
};

}
}

#endif
