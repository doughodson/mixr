
#ifndef __mixr_base_CubicFeet_HPP__
#define __mixr_base_CubicFeet_HPP__

#include "mixr/base/qty/volume/IVolume.hpp"

#include "mixr/base/qty/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CubicFeet
// Description: Concrete class to specify a volume in cubit feet
//------------------------------------------------------------------------------
class CubicFeet final: public IVolume
{
   DECLARE_SUBCLASS(CubicFeet, IVolume)

public:
   explicit CubicFeet();

private:
   // convert cubic feet to cubic meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * volume::CFT2CM; }
};

}
}

#endif
