
#ifndef __mixr_base_IFrequency_HPP__
#define __mixr_base_IFrequency_HPP__

#include "mixr/base/qty/IQuantity.hpp"

#include "mixr/base/qty/util/frequency_utils.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: IFrequency
// Description: Interface class for frequency. Base unit is Hertz.
//------------------------------------------------------------------------------
class IFrequency : public IQuantity
{
   DECLARE_SUBCLASS(IFrequency, IQuantity)

public:
   explicit IFrequency();

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
