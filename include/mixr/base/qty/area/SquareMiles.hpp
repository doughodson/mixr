
#ifndef __mixr_base_SquareMiles_HPP__
#define __mixr_base_SquareMiles_HPP__

#include "mixr/base/qty/area/IArea.hpp"

#include "mixr/base/qty/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareMiles
// Description: Square Meters * 0.00000038610216
//------------------------------------------------------------------------------
class SquareMiles final: public IArea
{
    DECLARE_SUBCLASS(SquareMiles, IArea)

public:
    explicit SquareMiles();

private:
   // convert square miles to square meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * area::SMI2SM; }
};

}
}

#endif
