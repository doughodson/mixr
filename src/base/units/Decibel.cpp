
#include "mixr/base/units/Decibel.hpp"
#include "mixr/base/numeric/Number.hpp"

#include <cmath>
#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Decibel, "dB")

BEGIN_SLOTTABLE(Decibel)
    "value",
END_SLOTTABLE(Decibel)

BEGIN_SLOT_MAP(Decibel)
    ON_SLOT(1, setSlotValue, Number)
END_SLOT_MAP()

Decibel::Decibel()
{
   STANDARD_CONSTRUCTOR()
}

Decibel::Decibel(const double value)
{
   STANDARD_CONSTRUCTOR()
   setValueDB(value);
}

void Decibel::copyData(const Decibel& org, const bool)
{
   BaseClass::copyData(org);
   db = org.db;
}

EMPTY_DELETEDATA(Decibel)

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
void Decibel::setValueDB(const double v)
{
   db = v;
   val = std::pow( static_cast<double>(10.0), static_cast<double>(db/10.0) );
}

void Decibel::setValue(const double v)
{
   BaseClass::setValue(v);
   db = std::log10(v) * 10.0;
}

bool Decibel::setSlotValue(const Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setValueDB( msg->getReal() );
      ok = true;
   }
   return ok;
}

}
}
