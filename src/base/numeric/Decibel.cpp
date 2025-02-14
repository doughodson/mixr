
#include "mixr/base/numeric/Decibel.hpp"

#include "mixr/base/numeric/INumber.hpp"

#include <cmath>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Decibel, "dB")
EMPTY_DELETEDATA(Decibel)

BEGIN_SLOTTABLE(Decibel)
    "value",
END_SLOTTABLE(Decibel)

BEGIN_SLOT_MAP(Decibel)
    ON_SLOT(1, setSlotValue, INumber)      // set the dB value
END_SLOT_MAP()

Decibel::Decibel()
{
   STANDARD_CONSTRUCTOR()
}

Decibel::Decibel(const double value)
{
   STANDARD_CONSTRUCTOR()
   setValuedB(value);
}

void Decibel::copyData(const Decibel& org, const bool)
{
   BaseClass::copyData(org);
   value = org.value;
   db = org.db;
}

void Decibel::setValuedB(const double v)
{
   db = v;
   value = std::pow( static_cast<double>(10.0), static_cast<double>(db/10.0) );
}

void Decibel::setValue(const double v)
{
//   BaseClass::setValue(v);
   value = v;
   db = std::log10(v) * 10.0;
}

bool Decibel::setSlotValue(const INumber* const x)
{
   setValuedB(x->asDouble());
   return true;
}

}
}
