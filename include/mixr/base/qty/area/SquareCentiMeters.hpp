
#ifndef __mixr_base_SquareCentiMeters_HPP__
#define __mixr_base_SquareCentiMeters_HPP__

#include "mixr/base/qty/area/IArea.hpp"

#include "mixr/base/qty/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareCentiMeters
// Description: Square Meters * 10000.0
//------------------------------------------------------------------------------
class SquareCentiMeters final: public IArea
{
    DECLARE_SUBCLASS(SquareCentiMeters, IArea)

public:
    explicit SquareCentiMeters();

private:
   // convert square centimeters to square meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * area::SCM2SM; }
};

}
}

#endif
