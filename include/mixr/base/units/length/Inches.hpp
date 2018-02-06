
#ifndef __mixr_base_Inches_H__
#define __mixr_base_Inches_H__

#include "mixr/base/units/length/Length.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Inches
// Description: Concrete class to specify length in terms of inches
//------------------------------------------------------------------------------
class Inches final: public Length
{
   DECLARE_SUBCLASS(Inches, Length)

public:
   explicit Inches();

private:
   // convert inches to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::IN2M; }
};

}
}

#endif
