
#ifndef __mixr_base_MicroMeters_HPP__
#define __mixr_base_MicroMeters_HPP__

#include "mixr/base/qty/length/ILength.hpp"

#include "mixr/base/qty/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MicroMeters
// Description: Concrete class to specify length in terms of micrometers
//------------------------------------------------------------------------------
class MicroMeters final: public ILength
{
   DECLARE_SUBCLASS(MicroMeters, ILength)

public:
   explicit MicroMeters();

private:
   // convert micrometers to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::UM2M; }
};

}
}

#endif
