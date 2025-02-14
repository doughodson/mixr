
#include "mixr/base/linearsystem/Sz1.hpp"
#include "mixr/base/numeric/INumber.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Sz1, "Sz1")
EMPTY_COPYDATA(Sz1)
EMPTY_DELETEDATA(Sz1)

BEGIN_SLOTTABLE(Sz1)
    "n1",      //  1: N1
    "N1",      //  2: N1
    "n2",      //  3: N2
    "N2",      //  4: N2
    "d1",      //  5: D1
    "D1",      //  6: D1
    "d2",      //  7: D2
    "D2",      //  8: D2
END_SLOTTABLE(Sz1)

BEGIN_SLOT_MAP(Sz1)
   ON_SLOT( 1, setSlotN1,   base::INumber)
   ON_SLOT( 2, setSlotN1,   base::INumber)
   ON_SLOT( 3, setSlotN2,   base::INumber)
   ON_SLOT( 4, setSlotN2,   base::INumber)
   ON_SLOT( 5, setSlotD1,   base::INumber)
   ON_SLOT( 6, setSlotD1,   base::INumber)
   ON_SLOT( 7, setSlotD2,   base::INumber)
   ON_SLOT( 8, setSlotD2,   base::INumber)
END_SLOT_MAP()

Sz1::Sz1()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool Sz1::setSlotN1(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setN1( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool Sz1::setSlotN2(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setN2( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool Sz1::setSlotD1(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setD1( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool Sz1::setSlotD2(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setD2( msg->asDouble() );
      ok = true;
   }
   return ok;
}

}
}
