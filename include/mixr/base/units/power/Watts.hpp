
#ifndef __mixr_base_Watts_H__
#define __mixr_base_Watts_H__

#include "mixr/base/units/power/Power.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: Watts
// Description: Concrete class to specify power in Watts
//----------------------------------------------------------------------------
class Watts final: public Power
{
   DECLARE_SUBCLASS(Watts, Power)

public:
   explicit Watts();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
