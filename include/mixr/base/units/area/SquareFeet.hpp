
#ifndef __mixr_base_SquareFeet_HPP__
#define __mixr_base_SquareFeet_HPP__

#include "mixr/base/units/area/IArea.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareFeet
// Description: Square Meters * 10.76391
//------------------------------------------------------------------------------
class SquareFeet final: public IArea
{
    DECLARE_SUBCLASS(SquareFeet, IArea)

public:
    explicit SquareFeet();

private:
   // convert square feet to square meters (our base unit)
   double convertToBaseUnit(const double x) final   { return x* area::SFT2SM; }
};

}
}

#endif
