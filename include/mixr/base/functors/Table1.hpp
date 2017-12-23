
#ifndef __mixr_base_Table1_H__
#define __mixr_base_Table1_H__

#include "mixr/base/functors/Table.hpp"

namespace mixr {
namespace base {
class FStorage;
class List;
class Number;

//------------------------------------------------------------------------------
// Class: Table1
//
// Description: 1D LFI data table
//
// Factory name: Table1
// Slots:
//    x    <List>  Independent variable #1 (iv1) points
//------------------------------------------------------------------------------
class Table1 : public Table
{
    DECLARE_SUBCLASS(Table1, Table)

public:
   Table1();
   Table1(const double* dtbl, const unsigned int dsize,
             const double* xtbl, const unsigned int xsize);

   // Returns the number of x breakpoints
   unsigned int getNumXPoints() const { return nx; }

   // Returns a pointer to the breakpoint data for x
   const double* getXData() const     { return xtable; }

   double getMinX() const;    // Min value of the X (iv1) breakpoints
   double getMaxX() const;    // Max value of the X (iv1) breakpoints

   // 1D Linear Function Interpolator: returns the result of f(x) using linear interpolation
   virtual double lfi(const double iv1, FStorage* const s = nullptr) const;

   // Load the X (iv1) breakpoints
   virtual bool setXBreakpoints1(const List* const bkpts);

   unsigned int tableSize() const override;

   bool isValid() const override;

protected:
   bool loadData(const List& list, double* const table) override;

private:
   double* xtable {};    // X Breakpoint Table
   unsigned int nx {};   // Number of x breakpoints
};

}
}

#endif
