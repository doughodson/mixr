
#ifndef __mixr_base_Liters_HPP__
#define __mixr_base_Liters_HPP__

#include "mixr/base/qty/volume/IVolume.hpp"

#include "mixr/base/qty/util/volume_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Liters
// Description: Concrete class to specify a volume in liters
//------------------------------------------------------------------------------
class Liters final: public IVolume
{
   DECLARE_SUBCLASS(Liters, IVolume)

public:
   explicit Liters();

private:
   // convert liters to cubic meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * volume::L2CM; }
};

}
}

#endif
