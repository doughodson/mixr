
#include "mixr/models/signature/SigAzEl.hpp"

#include "mixr/base/relations/Table2.hpp"
#include "mixr/base/numeric/Boolean.hpp"

#include "mixr/models/Emission.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SigAzEl, "SigAzEl")

BEGIN_SLOTTABLE(SigAzEl)
    "table",            // 1: Table of RCS by target Az/El angles  (base::Table2)
    "swapOrder",        // 2: True if elevation is the table's first independent
                        //    variable and azimuth is the second.
    "inDegrees",        // 3: True if the table's independent variables az and
                        //    el are in degrees instead of the default radians
    "inDecibel",        // 4: True if the dependent data is in decibel meters
                        //    squared instead of the default meters squared
END_SLOTTABLE(SigAzEl)

BEGIN_SLOT_MAP(SigAzEl)
    ON_SLOT(1, setSlotTable,        base::Table2)
    ON_SLOT(2, setSlotSwapOrder,    base::Boolean)
    ON_SLOT(3, setSlotInDegrees,    base::Boolean)
    ON_SLOT(4, setSlotDecibel,      base::Boolean)
END_SLOT_MAP()

SigAzEl::SigAzEl()
{
   STANDARD_CONSTRUCTOR()
}

SigAzEl::SigAzEl(const base::Table2* const tbl0)
{
   STANDARD_CONSTRUCTOR()

   if (tbl0 != nullptr) {
      tbl = tbl0->clone();
   }
}

void SigAzEl::copyData(const SigAzEl& org, const bool)
{
   BaseClass::copyData(org);

   if (tbl != nullptr) { tbl->unref(); tbl = nullptr; }
   if (org.tbl != nullptr) {
      tbl = org.tbl->clone();
   }

   swapOrderFlg = org.swapOrderFlg;
   degFlg = org.degFlg;
   dbFlg = org.dbFlg;
}

void SigAzEl::deleteData()
{
    if (tbl != nullptr) { tbl->unref(); tbl = nullptr; }
}

//------------------------------------------------------------------------------
// getRCS() -- Get the RCS
//------------------------------------------------------------------------------
double SigAzEl::getRCS(const Emission* const em)
{
   double rcs{};
   if (em != nullptr && tbl != nullptr) {

      // angle of arrival (radians)
      double iv1{em->getAzimuthAoi()};
      double iv2{em->getElevationAoi()};

      // If the table's independent variable's order is swapped: (El, Az)
      if (isOrderSwapped()) {
         iv1 = em->getElevationAoi();
         iv2 = em->getAzimuthAoi();
      }

      // If the table's independent variables are in degrees ..
      if (isInDegrees()) {
         iv1 *= static_cast<double>(base::angle::R2DCC);
         iv2 *= static_cast<double>(base::angle::R2DCC);
      }

      rcs = tbl->lfi(iv1,iv2);

      // If the dependent data is in decibels ...
      if (isDecibel()) {
         rcs = std::pow(static_cast<double>(10.0), static_cast<double>(rcs / 10.0));
      }
   }
   return rcs;
}

//------------------------------------------------------------------------------
// isTableValid() -- Returns true if this signature has a good az/el table
//------------------------------------------------------------------------------
bool SigAzEl::isTableValid() const
{
   bool ok{};
   if (tbl != nullptr) {
      ok = tbl->isValid();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool SigAzEl::setSwapOrder(const bool flg)
{
   swapOrderFlg = flg;
   return true;
}

bool SigAzEl::setInDegrees(const bool flg)
{
   degFlg = flg;
   return true;
}

bool SigAzEl::setDecibel(const bool flg)
{
   dbFlg = flg;
   return true;
}

// Sets the signature table
bool SigAzEl::setSlotTable(const base::Table2* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      if (tbl != nullptr) tbl->unref();
      msg->ref();
      tbl = msg;
      ok = true;
   }
   return ok;
}

bool SigAzEl::setSlotSwapOrder(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setSwapOrder( msg->to_bool() );
   }
   return ok;
}

bool SigAzEl::setSlotInDegrees(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setInDegrees( msg->to_bool() );
   }
   return ok;
}

bool SigAzEl::setSlotDecibel(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setDecibel( msg->to_bool() );
   }
   return ok;
}

}
}
