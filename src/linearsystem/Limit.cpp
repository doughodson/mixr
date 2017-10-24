
#include "mixr/linearsystem/Limit.hpp"
#include "mixr/base/numeric/Number.hpp"
#include <iostream>

namespace mixr {
namespace linearsystem {

IMPLEMENT_SUBCLASS(Limit, "Limit")
EMPTY_COPYDATA(Limit)
EMPTY_DELETEDATA(Limit)

BEGIN_SLOTTABLE(Limit)
    "lower",   //  1: Lower boundary (default: -1) (must be less than the upper)
    "upper",   //  2: Upper boundary (default:  1) (must be greater than the lower)
END_SLOTTABLE(Limit)

BEGIN_SLOT_MAP(Limit)
   ON_SLOT( 1, setSlotLowerLimit, base::Number)
   ON_SLOT( 2, setSlotUpperLimit, base::Number)
END_SLOT_MAP()

Limit::Limit()
{
   STANDARD_CONSTRUCTOR()
}

Limit::Limit(const double ll, const double uu) : LimitFunc(ll,uu)
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool Limit::setSlotLowerLimit(const base::Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setLowerLimit( msg->getReal() );
      ok = true;
   }
   return ok;
}

bool Limit::setSlotUpperLimit(const base::Number* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setUpperLimit( msg->getReal() );
      ok = true;
   }
   return ok;
}

}
}
