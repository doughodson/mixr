
#ifndef __mixr_base_IArea_HPP__
#define __mixr_base_IArea_HPP__

#include "mixr/base/qty/IQuantity.hpp"

#include "mixr/base/qty/util/area_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: IArea
// Description: Interface class for area - base unit is a square meter (m^2)
//------------------------------------------------------------------------------
class IArea : public IQuantity
{
   DECLARE_SUBCLASS(IArea, IQuantity)

public:
   explicit IArea();

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
