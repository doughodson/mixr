
#ifndef __mixr_base_ILength_HPP__
#define __mixr_base_ILength_HPP__

#include "mixr/base/qty/IQuantity.hpp"

#include "mixr/base/qty/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: ILength
// Description: Interface class for length.  Base unit is meters.
//------------------------------------------------------------------------------
class ILength : public IQuantity
{
   DECLARE_SUBCLASS(ILength, IQuantity)

public:
   explicit ILength();

   // conversion methods
   double getValueInCentiMeters() const    { return getValue() * length::M2CM; }
   double getValueInFeet() const           { return getValue() * length::M2FT; }
   double getValueInInches() const         { return getValue() * length::M2IN; }
   double getValueInKiloMeters() const     { return getValue() * length::M2KM; }
   double getValueInMeters() const         { return getValue();                }
   double getValueInMicroMeters() const    { return getValue() * length::M2UM; }
   double getValueInMicrons() const        { return getValue() * length::M2UM; }
   double getValueInNauticalMiles() const  { return getValue() * length::M2NM; }
   double getValueInStatuteMiles() const   { return getValue() * length::M2SM; }
};

}
}

#endif
