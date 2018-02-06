
#ifndef __mixr_base_SquareFeet_H__
#define __mixr_base_SquareFeet_H__

#include "mixr/base/units/area/Area.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareFeet
// Description: Square Meters * 10.76391
//------------------------------------------------------------------------------
class SquareFeet final: public Area
{
    DECLARE_SUBCLASS(SquareFeet, Area)

public:
    explicit SquareFeet();

private:
   // convert square feet to square meters (our base unit)
   double convertToBaseUnit(const double x) final   { return x* area::SFT2SM; }
};

}
}

#endif
