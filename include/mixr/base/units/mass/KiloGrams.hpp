
#ifndef __mixr_base_KiloGrams_HPP__
#define __mixr_base_KiloGrams_HPP__

#include "mixr/base/units/mass/IMass.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: KiloGrams
// Description: Concrete class to specify a mass in KiloGrams
//------------------------------------------------------------------------------
class KiloGrams final: public IMass
{
   DECLARE_SUBCLASS(KiloGrams, IMass)

public:
   explicit KiloGrams();

private:
   // convert KiloGrams to KiloGrams (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
