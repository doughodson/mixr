
#ifndef __mixr_base_CubicMeters_H__
#define __mixr_base_CubicMeters_H__

#include "mixr/base/units/volume/Volume.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: CubicMeters
// Description: Concrete class to specify a volume in cubic meters (m^3).
//------------------------------------------------------------------------------
class CubicMeters final: public Volume
{
   DECLARE_SUBCLASS(CubicMeters, Volume)

public:
   explicit CubicMeters();

private:
   // convert cubic meters to cubic meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
