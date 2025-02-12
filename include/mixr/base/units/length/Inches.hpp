
#ifndef __mixr_base_Inches_HPP__
#define __mixr_base_Inches_HPP__

#include "mixr/base/units/length/ILength.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Inches
// Description: Concrete class to specify length in terms of inches
//------------------------------------------------------------------------------
class Inches final: public ILength
{
   DECLARE_SUBCLASS(Inches, ILength)

public:
   explicit Inches();

private:
   // convert inches to meters (our base unit)
   double convertToBaseUnit(const double x) final     { return x * length::IN2M; }
};

}
}

#endif
