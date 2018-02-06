
#ifndef __mixr_base_CubicFeet_H__
#define __mixr_base_CubicFeet_H__

#include "mixr/base/units/volume/Volume.hpp"

#include "mixr/base/units/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CubicFeet
// Description: Concrete class to specify a volume in cubit feet
//------------------------------------------------------------------------------
class CubicFeet final: public Volume
{
   DECLARE_SUBCLASS(CubicFeet, Volume)

public:
   explicit CubicFeet();

private:
   // convert cubic feet to cubic meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * volume::CFT2CM; }
};

}
}

#endif
