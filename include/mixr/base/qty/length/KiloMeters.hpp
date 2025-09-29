
#ifndef __mixr_base_KiloMeters_HPP__
#define __mixr_base_KiloMeters_HPP__

#include "mixr/base/qty/length/ILength.hpp"

#include "mixr/base/qty/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloMeters
// Description: Concrete class to specify length in terms of kilometers
//------------------------------------------------------------------------------
class KiloMeters final: public ILength
{
   DECLARE_SUBCLASS(KiloMeters, ILength)

public:
   explicit KiloMeters();

private:
   // convert kilometers to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::KM2M; }
};

}
}

#endif
