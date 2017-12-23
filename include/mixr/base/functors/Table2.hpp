
#ifndef __mixr_base_Table2_H__
#define __mixr_base_Table2_H__

#include "mixr/base/functors/Table1.hpp"

namespace mixr {
namespace base {
class FStorage;
class List;

//------------------------------------------------------------------------------
// Class: Table2
//
// Description: 2D LFI data table
//
// Factory name: Table2
// Slots:
//    y    <List>  Independent variable #2 (iv2) points
//
//------------------------------------------------------------------------------
class Table2 : public Table1
{
    DECLARE_SUBCLASS(Table2, Table1)

public:
   Table2();
   Table2(const double* dtbl, const unsigned int dsize,
          const double* xtbl, const unsigned int xsize,
          const double* ytbl, const unsigned int ysize);

   // Returns the number of y breakpoints
   unsigned int getNumYPoints() const { return ny; }

   // Returns a pointer to the breakpoint data for y
   const double* getYData() const     { return ytable; }

   double getMinY() const;    // Min value of the Y (iv2) breakpoints
   double getMaxY() const;    // Max value of the Y (iv2) breakpoints

   // 2D Linear Function Interpolator: returns the result of f(x,y) using linear interpolation
   virtual double lfi(const double iv1, const double iv2, FStorage* const s = nullptr) const;

   // Load the Y (iv2) breakpoints
   virtual bool setYBreakpoints2(const List* const bkpts);

   double lfi(const double iv1, FStorage* const s = nullptr) const override;
   unsigned int tableSize() const override;

   bool isValid() const override;

protected:
   bool loadData(const List& list, double* const table) override;

private:
   double* ytable {};    // Y Breakpoint Table
   unsigned int ny {};   // Number of y breakpoints
};

}
}

#endif
