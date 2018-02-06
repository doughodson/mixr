
#ifndef __mixr_base_MicroMeters_H__
#define __mixr_base_MicroMeters_H__

#include "mixr/base/units/length/Length.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MicroMeters
// Description: Concrete class to specify length in terms of micrometers
//------------------------------------------------------------------------------
class MicroMeters final: public Length
{
   DECLARE_SUBCLASS(MicroMeters, Length)

public:
   explicit MicroMeters();

private:
   // convert micrometers to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::UM2M; }
};

}
}

#endif
