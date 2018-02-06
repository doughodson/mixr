
#ifndef __mixr_base_CubicInches_H__
#define __mixr_base_CubicInches_H__

#include "mixr/base/units/volume/Volume.hpp"

#include "mixr/base/units/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CubicInches
// Description: Concrete class to specify a volume in cubit inches
//------------------------------------------------------------------------------
class CubicInches final: public Volume
{
   DECLARE_SUBCLASS(CubicInches, Volume)

public:
   explicit CubicInches();

private:
   // convert cubic inches to cubic meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * volume::CIN2CM; }
};

}
}

#endif
