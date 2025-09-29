
#ifndef __mixr_base_Minutes_HPP__
#define __mixr_base_Minutes_HPP__

#include "mixr/base/qty/time/ITime.hpp"

#include "mixr/base/qty/util/time_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Minutes
// Description: Concrete class to specify a time in minutes
//------------------------------------------------------------------------------
class Minutes final: public ITime
{
   DECLARE_SUBCLASS(Minutes, ITime)

public:
   explicit Minutes();

private:
   // convert minutes to seconds (our base unit)
   double convertToBaseUnit(const double x) final     { return x * time::M2S; }
};

}
}

#endif
