
#ifndef __mixr_base_Table3_H__
#define __mixr_base_Table3_H__

#include "mixr/base/functors/Table2.hpp"

namespace mixr {
namespace base {
class FStorage;
class List;

//------------------------------------------------------------------------------
// Class: Table3
//
// Description: 3D LFI data table
//
// Factory name: Table3
// Slots:
//    z    <List>  Independent variable #3 (iv3) points
//
//------------------------------------------------------------------------------
class Table3 : public Table2
{
   DECLARE_SUBCLASS(Table3, Table2)

public:
   Table3();
   Table3(const double* dtbl, const unsigned int dsize,
          const double* xtbl, const unsigned int xsize,
          const double* ytbl, const unsigned int ysize,
          const double* ztbl, const unsigned int zsize);

   // Returns the number of z breakpoints.
   unsigned int getNumZPoints() const { return nz; }

   // double* getZData()
   const double* getZData() const     { return ztable; }

   double getMinZ() const;    // Min value of the Z (iv3) breakpoints
   double getMaxZ() const;    // Max value of the Z (iv3) breakpoints

   // 3D Linear Function Interpolator: returns the result of f(x,y,z) using linear interpolation
   virtual double lfi(const double iv1, const double iv2, const double iv3, FStorage* const s = nullptr) const;

   // Loads the Z (iv3) breakpoints
   virtual bool setZBreakpoints3(const List* const bkpts);

   double lfi(const double iv1, const double iv2, FStorage* const s = nullptr) const override;
   double lfi(const double iv1, FStorage* const s = nullptr) const override;
   unsigned int tableSize() const override;

   bool isValid() const override;

protected:
   bool loadData(const List& list, double* const table) override;

private:
   double* ztable {};    // Z Breakpoint Table
   unsigned int nz {};   // Number of z breakpoints
};

}
}

#endif
