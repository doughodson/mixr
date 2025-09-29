
#ifndef __mixr_base_CubicMeters_HPP__
#define __mixr_base_CubicMeters_HPP__

#include "mixr/base/qty/volume/IVolume.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CubicMeters
// Description: Concrete class to specify a volume in cubic meters (m^3).
//------------------------------------------------------------------------------
class CubicMeters final: public IVolume
{
   DECLARE_SUBCLASS(CubicMeters, IVolume)

public:
   explicit CubicMeters();

private:
   // convert cubic meters to cubic meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
