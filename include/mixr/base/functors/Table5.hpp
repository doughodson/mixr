
#ifndef __mixr_base_Table5_H__
#define __mixr_base_Table5_H__

#include "mixr/base/functors/Table4.hpp"

namespace mixr {
namespace base {
class FStorage;
class List;

//------------------------------------------------------------------------------
// Class: Table5
//
// Description: 5D LFI data table
//
// Factory name: Table5
// Slots:
//    v    <List>  Independent variable #5 (iv5) points
//
//------------------------------------------------------------------------------
class Table5 : public Table4
{
   DECLARE_SUBCLASS(Table5, Table4)

public:
   Table5();
   Table5(const double* dtbl, const unsigned int dsize,
          const double* xtbl, const unsigned int xsize,
          const double* ytbl, const unsigned int ysize,
          const double* ztbl, const unsigned int zsize,
          const double* wtbl, const unsigned int wsize,
          const double* vtbl, const unsigned int vsize);

   // Returns the number of v breakpoints
   unsigned int getNumVPoints() const { return nv; }

   // Returns a pointer to the breakpoint data for v
   const double* getVData() const     { return vtable; }

   double getMinV() const;    // Min value of the V (iv5) breakpoints
   double getMaxV() const;    // Max value of the V (iv5) breakpoints

   virtual double lfi(const double iv1, const double iv2, const double iv3, const double iv4, const double iv5, FStorage* const s = nullptr) const;

   // Loads the V (iv5) breakpoints
   virtual bool setVBreakpoints5(const List* const bkpts);

   double lfi(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const s = nullptr) const override;
   double lfi(const double iv1, const double iv2, const double iv3, FStorage* const s = nullptr) const override;
   double lfi(const double iv1, const double iv2, FStorage* const s = nullptr) const override;
   double lfi(const double iv1, FStorage* const s = nullptr) const override;
   unsigned int tableSize() const override;

   bool isValid() const override;

protected:
   bool loadData(const List& list, double* const table) override;

private:
   double* vtable {};     // V Breakpoint Table
   unsigned int nv {};    // Number of v breakpoints
};

}
}

#endif
