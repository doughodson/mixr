
#ifndef __mixr_base_Microns_HPP__
#define __mixr_base_Microns_HPP__

#include "mixr/base/qty/length/ILength.hpp"

#include "mixr/base/qty/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Microns (same as MicroMeters)
// Description: Concrete class to specify length in terms of microns
//------------------------------------------------------------------------------
class Microns final: public ILength
{
   DECLARE_SUBCLASS(Microns, ILength)

public:
   explicit Microns();

private:
   // convert microns to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::UM2M; }
};

}
}

#endif
