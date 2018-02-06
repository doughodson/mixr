
#ifndef __mixr_base_Distance_H__
#define __mixr_base_Distance_H__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/length_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Length
// Description: Abstract class for length.  Base unit is meters.
//------------------------------------------------------------------------------
class Length : public Quantity
{
   DECLARE_SUBCLASS(Length, Quantity)

public:
   explicit Length();

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
