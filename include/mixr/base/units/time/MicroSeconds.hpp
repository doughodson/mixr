
#ifndef __mixr_base_MicroSeconds_H__
#define __mixr_base_MicroSeconds_H__

#include "mixr/base/units/time/Time.hpp"

#include "mixr/base/units/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: MicroSeconds
// Description: Concrete class to specify a time in microseconds
//------------------------------------------------------------------------------
class MicroSeconds final: public Time
{
   DECLARE_SUBCLASS(MicroSeconds, Time)

public:
   explicit MicroSeconds();

private:
   // convert microseconds to seconds (our base unit)
   double convertToBaseUnit(const double x) final     { return x * time::US2S; }
};

}
}

#endif
