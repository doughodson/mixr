
#ifndef __mixr_models_common_RfAzElSignature_HPP__
#define __mixr_models_common_RfAzElSignature_HPP__

#include "mixr/models/signature/IRfSignature.hpp"

namespace mixr {
namespace base { class Boolean; class Table2; }
namespace models {
class RfEmission;

//------------------------------------------------------------------------------
// Class: RfAzElSignature
// Descriptions: Concrete RF signature by a table of RCS based on target's az/el angles
//
// Factory name: RfAzElSignature
// Slots:
//
//    table      <base::Table2>   ! Table of RCS by target Az/El angles (default: 0)
//
//    swapOrder  <base::Boolean>  ! True if elevation is the table's first independent (default: false)
//                                ! variable and azimuth is the second.
//
//    inDegrees  <base::Boolean>  ! True if the table's independent variables az and
//                                ! el are in degrees instead of the default radians (default: false)
//
//    inDecibel  <base::Boolean>  ! True if the dependent data is in decibel meters
//                                ! squared instead of the default meters squared (default: false)
//
// Notes:
//  1) Must provide a base::Table2 (2 dimensional) table, where ...
//       -- Azimuth is the first independent variable (radians),
//       -- Elevation is the second independent variable (radians),
//       -- and the RCS is the dependent variable (meters squared)
//
//  2) If 'swapOrder' is set true then elevation is the table's first independent
//     variable and azimuth is the second.
//
//  3) If 'inDegrees' is set true then the table's independent variables az and
//     el are in degrees instead of the default radians
//
//  4) If 'inDecibel' is set true then the dependent data is in decibel meters
//     squared instead of the default meters squared
//------------------------------------------------------------------------------
class RfAzElSignature final: public IRfSignature
{
    DECLARE_SUBCLASS(RfAzElSignature, IRfSignature)
public:
   RfAzElSignature();
   RfAzElSignature(const base::Table2* const);

   virtual bool isTableValid() const;

   bool isOrderSwapped() const                  { return swapOrderFlg; }
   virtual bool setSwapOrder(const bool);

   bool isInDegrees() const                     { return degFlg; }
   virtual bool setInDegrees(const bool);

   bool isDecibel() const                       { return dbFlg; }
   virtual bool setDecibel(const bool);

   double getRCS(const RfEmission* const em) override;

protected:
   const base::Table2* tbl{};      // The table
   bool swapOrderFlg{};            // Swap independent data order from az/el to el/az
   bool degFlg{};                  // independent data in degrees
   bool dbFlg{};                   // dependent data in decibels

private:
   // slot table helper methods
   bool setSlotTable(const base::Table2* const);
   bool setSlotSwapOrder(const base::Boolean* const);
   bool setSlotInDegrees(const base::Boolean* const);
   bool setSlotDecibel(const base::Boolean* const);
};

}
}

#endif
