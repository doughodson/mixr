
#ifndef __mixr_base_NanoSeconds_HPP__
#define __mixr_base_NanoSeconds_HPP__

#include "mixr/base/units/time/ITime.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: NanoSeconds
// Description: Concrete class to specify a time in nanoseconds
//------------------------------------------------------------------------------
class NanoSeconds final: public ITime
{
   DECLARE_SUBCLASS(NanoSeconds, ITime)

public:
   explicit NanoSeconds();

private:
   // convert nanoseconds to seconds (our base unit)
   double convertToBaseUnit(const double x) final     { return x * time::NS2S; }
};

}
}

#endif
