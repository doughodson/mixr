
#ifndef __mixr_base_SquareYards_HPP__
#define __mixr_base_SquareYards_HPP__

#include "mixr/base/qty/area/IArea.hpp"

#include "mixr/base/qty/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareYards
// Description: Square Meters * 1.19599
//------------------------------------------------------------------------------
class SquareYards final: public IArea
{
    DECLARE_SUBCLASS(SquareYards, IArea)

public:
    explicit SquareYards();

private:
   // convert square yards to square meters (our base unit)
   double convertToBaseUnit(const double x) final           { return x * area::SYD2SM; }
};

}
}

#endif
