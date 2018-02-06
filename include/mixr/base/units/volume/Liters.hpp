
#ifndef __mixr_base_Liters_H__
#define __mixr_base_Liters_H__

#include "mixr/base/units/volume/Volume.hpp"

#include "mixr/base/units/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Liters
// Description: Concrete class to specify a volume in liters
//------------------------------------------------------------------------------
class Liters final: public Volume
{
   DECLARE_SUBCLASS(Liters, Volume)

public:
   explicit Liters();

private:
   // convert liters to cubic meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * volume::L2CM; }
};

}
}

#endif
