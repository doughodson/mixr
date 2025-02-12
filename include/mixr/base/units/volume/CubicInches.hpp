
#ifndef __mixr_base_CubicInches_HPP__
#define __mixr_base_CubicInches_HPP__

#include "mixr/base/units/volume/IVolume.hpp"

#include "mixr/base/units/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CubicInches
// Description: Concrete class to specify a volume in cubit inches
//------------------------------------------------------------------------------
class CubicInches final: public IVolume
{
   DECLARE_SUBCLASS(CubicInches, IVolume)

public:
   explicit CubicInches();

private:
   // convert cubic inches to cubic meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * volume::CIN2CM; }
};

}
}

#endif
