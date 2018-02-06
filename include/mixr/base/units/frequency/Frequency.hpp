
#ifndef __mixr_base_Frequency_H__
#define __mixr_base_Frequency_H__

#include "mixr/base/units/Quantity.hpp"

#include "mixr/base/units/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Frequency
// Description: Abstract class for frequency. Base unit is Hertz.
//------------------------------------------------------------------------------
class Frequency : public Quantity
{
   DECLARE_SUBCLASS(Frequency, Quantity)

public:
   explicit Frequency();

   // conversion methods
   double getValueInGigaHertz() const   { return getValue() * frequency::Hz2GHz;  }
   double getValueInHertz() const       { return getValue();                      }
   double getValueInKiloHertz() const   { return getValue() * frequency::Hz2KHz;  }
   double getValueInMegaHertz() const   { return getValue() * frequency::Hz2MHz;  }
   double getValueInTeraHertz() const   { return getValue() * frequency::Hz2THz;  }
};

}
}

#endif
