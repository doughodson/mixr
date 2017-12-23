
#ifndef __mixr_base_Table4_H__
#define __mixr_base_Table4_H__

#include "mixr/base/functors/Table3.hpp"

namespace mixr {
namespace base {
class FStorage;
class List;

//------------------------------------------------------------------------------
// Class: Table4
//
// Description: 4D LFI data table
//
// Factory name: Table4
// Slots:
//    w    <List>  Independent variable #4 (iv4) points
//
//------------------------------------------------------------------------------
class Table4 : public Table3
{
   DECLARE_SUBCLASS(Table4, Table3)

public:
   Table4();
   Table4(const double* dtbl, const unsigned int dsize,
          const double* xtbl, const unsigned int xsize,
          const double* ytbl, const unsigned int ysize,
          const double* ztbl, const unsigned int zsize,
          const double* wtbl, const unsigned int wsize);

   // Returns the number of w breakpoints.
   unsigned int getNumWPoints() const { return nw; }

   // Returns a pointer to the breakpoint data for w
   const double* getWData() const     { return wtable; }

   double getMinW() const;    // Min value of the W (iv4) breakpoints
   double getMaxW() const;    // Max value of the W (iv4) breakpoints

   // 4D Linear Function Interpolator: returns the result of f(x,y,z,w) using linear interpolation
   virtual double lfi(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const s = nullptr) const;

   // Loads the W (iv4) breakpoints
   virtual bool setWBreakpoints4(const List* const bkpts);

   double lfi(const double iv1, const double iv2, const double iv3, FStorage* const s = nullptr) const override;
   double lfi(const double iv1, const double iv2, FStorage* const s = nullptr) const override;
   double lfi(const double iv1, FStorage* const s = nullptr) const override;
   unsigned int tableSize() const override;

   bool isValid() const override;

protected:
   bool loadData(const List& list, double* const table) override;

private:
   double* wtable {};    // W Breakpoint Table
   unsigned int nw {};   // Number of w breakpoints
};

}
}

#endif
