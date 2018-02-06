
#ifndef __mixr_base_SquareYards_H__
#define __mixr_base_SquareYards_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareYards
// Description: Square Meters * 1.19599
//------------------------------------------------------------------------------
class SquareYards final: public Area
{
    DECLARE_SUBCLASS(SquareYards, Area)

public:
    explicit SquareYards();

private:
   // convert square yards to square meters (our base unit)
   double convertToBaseUnit(const double x) final           { return x * area::SYD2SM; }
};

}
}

#endif
