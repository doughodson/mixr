
#ifndef __mixr_base_Watts_HPP__
#define __mixr_base_Watts_HPP__

#include "mixr/base/qty/power/IPower.hpp"

namespace mixr {
namespace base {

//----------------------------------------------------------------------------
// Class: Watts
// Description: Concrete class to specify power in Watts
//----------------------------------------------------------------------------
class Watts final: public IPower
{
   DECLARE_SUBCLASS(Watts, IPower)

public:
   explicit Watts();

private:
   // convert X to Y (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
