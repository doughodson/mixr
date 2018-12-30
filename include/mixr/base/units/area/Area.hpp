
#ifndef __mixr_base_Area_HPP__
#define __mixr_base_Area_HPP__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Area
// Description: Abstract class for area. Base unit is a square meter (m^2).
//------------------------------------------------------------------------------
class Area : public Quantity
{
   DECLARE_SUBCLASS(Area, Quantity)

public:
   explicit Area();

   // conversion methods
   double getValueInDecibelSquareMeters() const  { return 10.0 * std::log10(getValue()); }
   double getValueInSquareCentiMeters() const    { return getValue() * area::SM2SCM;     }
   double getValueInSquareFeet() const           { return getValue() * area::SM2SFT;     }
   double getValueInSquareInches() const         { return getValue() * area::SM2SIN;     }
   double getValueInSquareKiloMeters() const     { return getValue() * area::SM2SKM;     }
   double getValueInSquareMeters() const         { return getValue();                    }
   double getValueInSquareMiles() const          { return getValue() * area::SM2SMI;     }
   double getValueInSquareMilliMeters() const    { return getValue() * area::SM2SMM;     }
   double getValueInSquareYards() const          { return getValue() * area::SM2SYD;     }
};

}
}

#endif
