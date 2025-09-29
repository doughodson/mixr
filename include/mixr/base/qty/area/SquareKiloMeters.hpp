
#ifndef __mixr_base_SquareKiloMeters_HPP__
#define __mixr_base_SquareKiloMeters_HPP__

#include "mixr/base/qty/area/IArea.hpp"

#include "mixr/base/qty/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareKiloMeters
// Description: Square Meters * 0.000001
//------------------------------------------------------------------------------
class SquareKiloMeters final: public IArea
{
    DECLARE_SUBCLASS(SquareKiloMeters, IArea)

public:
    explicit SquareKiloMeters();

private:
   // convert square kilometers to square meters (our base unit)
   double convertToBaseUnit(const double x) final   { return x * area::SKM2SM; }
};

}
}

#endif
