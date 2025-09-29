
#ifndef __mixr_base_DecibelSquareMeters_HPP__
#define __mixr_base_DecibelSquareMeters_HPP__

#include "mixr/base/qty/area/IArea.hpp"

#include "mixr/base/qty/util/area_utils.hpp"
#include <cmath>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: DecibelSquareMeters
// Description: 10 * Log(Square Meters)
//------------------------------------------------------------------------------
class DecibelSquareMeters final: public IArea
{
    DECLARE_SUBCLASS(DecibelSquareMeters, IArea)

public:
    explicit DecibelSquareMeters();

private:
   // convert decibel square meters to square meters (our base unit)
   double convertToBaseUnit(const double x) final    { return std::pow(10.0, x/10.0); }
};

}
}

#endif
