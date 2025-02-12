
#ifndef __mixr_base_SquareInches_HPP__
#define __mixr_base_SquareInches_HPP__

#include "mixr/base/units/area/IArea.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareInches
// Description: Square Meters * 1550.0030399
//------------------------------------------------------------------------------
class SquareInches final: public IArea
{
    DECLARE_SUBCLASS(SquareInches, IArea)

public:
    explicit SquareInches();

private:
   // convert square inches to square meters (our base unit)
   double convertToBaseUnit(const double x) final   { return x * area::SIN2SM; }
};

}
}

#endif
