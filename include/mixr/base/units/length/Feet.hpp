
#ifndef __mixr_base_Feet_H__
#define __mixr_base_Feet_H__

#include "mixr/base/units/length/Length.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Feet
// Description: Concrete class to specify length in terms of feet
//------------------------------------------------------------------------------
class Feet final: public Length
{
   DECLARE_SUBCLASS(Feet, Length)

public:
   explicit Feet();

private:
   // convert feet to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::FT2M; }
};

}
}

#endif
