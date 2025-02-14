
#include "mixr/base/linearsystem/Sz2.hpp"
#include "mixr/base/numeric/INumber.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Sz2, "Sz2")
EMPTY_COPYDATA(Sz2)
EMPTY_DELETEDATA(Sz2)

BEGIN_SLOTTABLE(Sz2)
    "n1",      //  1: N1
    "N1",      //  2: N1
    "n2",      //  3: N2
    "N2",      //  4: N2
    "n3",      //  5: N2
    "N3",      //  6: N2
    "d1",      //  7: D1
    "D1",      //  8: D1
    "d2",      //  9: D2
    "D2",      // 10: D2
    "d3",      // 11: D2
    "D3",      // 12: D2
END_SLOTTABLE(Sz2)

BEGIN_SLOT_MAP(Sz2)
   ON_SLOT( 1, setSlotN1,   base::INumber)
   ON_SLOT( 2, setSlotN1,   base::INumber)
   ON_SLOT( 3, setSlotN2,   base::INumber)
   ON_SLOT( 4, setSlotN2,   base::INumber)
   ON_SLOT( 5, setSlotN3,   base::INumber)
   ON_SLOT( 6, setSlotN3,   base::INumber)
   ON_SLOT( 7, setSlotD1,   base::INumber)
   ON_SLOT( 8, setSlotD1,   base::INumber)
   ON_SLOT( 9, setSlotD2,   base::INumber)
   ON_SLOT(10, setSlotD2,   base::INumber)
   ON_SLOT(11, setSlotD3,   base::INumber)
   ON_SLOT(12, setSlotD3,   base::INumber)
END_SLOT_MAP()

Sz2::Sz2()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------

bool Sz2::setSlotN1(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setN1( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotN2(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setN2( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotN3(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setN3( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotD1(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setD1( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotD2(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setD2( msg->asDouble() );
      ok = true;
   }
   return ok;
}

bool Sz2::setSlotD3(const base::INumber* const msg)
{
   bool ok {};
   if (msg != nullptr) {
      setD3( msg->asDouble() );
      ok = true;
   }
   return ok;
}

}
}
