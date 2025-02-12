
#ifndef __mixr_base_Meters_HPP__
#define __mixr_base_Meters_HPP__

#include "mixr/base/units/length/ILength.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Meters
// Description: Concrete class to specify length in terms of meters
//------------------------------------------------------------------------------
class Meters final: public ILength
{
   DECLARE_SUBCLASS(Meters, ILength)

public:
   explicit Meters();

private:
   // convert meters to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
