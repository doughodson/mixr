
#ifndef __mixr_base_SquareMeters_HPP__
#define __mixr_base_SquareMeters_HPP__

#include "mixr/base/qty/area/IArea.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SquareMeters
// Description: Concrete class to specify an area in qty of square meters (m^2)
//------------------------------------------------------------------------------
class SquareMeters final: public IArea
{
   DECLARE_SUBCLASS(SquareMeters, IArea)

public:
   explicit SquareMeters();

private:
   // convert square meters to square meters (our base unit)
   double convertToBaseUnit(const double x) final           { return x; }
};

}
}

#endif
