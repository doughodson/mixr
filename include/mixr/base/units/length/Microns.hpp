
#ifndef __mixr_base_Microns_H__
#define __mixr_base_Microns_H__

#include "mixr/base/units/length/Length.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Microns (same as MicroMeters)
// Description: Concrete class to specify length in terms of microns
//------------------------------------------------------------------------------
class Microns final: public Length
{
   DECLARE_SUBCLASS(Microns, Length)

public:
   explicit Microns();

private:
   // convert microns to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::UM2M; }
};

}
}

#endif
