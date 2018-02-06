
#ifndef __mixr_base_SquareMiles_H__
#define __mixr_base_SquareMiles_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareMiles
// Description: Square Meters * 0.00000038610216
//------------------------------------------------------------------------------
class SquareMiles final: public Area
{
    DECLARE_SUBCLASS(SquareMiles, Area)

public:
    explicit SquareMiles();

private:
   // convert square miles to square meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * area::SMI2SM; }
};

}
}

#endif
