
#include "mixr/base/functors/Table1.hpp"

#include "mixr/base/functors/TableStorage.hpp"
#include "mixr/base/util/lfi.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Table1, "Table1")

BEGIN_SLOTTABLE(Table1)
    "x",           // X breakpoints
END_SLOTTABLE(Table1)

BEGIN_SLOT_MAP(Table1)
    ON_SLOT(1, setXBreakpoints1, List)
END_SLOT_MAP()

Table1::Table1() : Table()
{
   STANDARD_CONSTRUCTOR()
}

Table1::Table1(const double* dtbl, const unsigned int dsize,
                   const double* xtbl, const unsigned int xsize)
                   : Table(dtbl, dsize)
{
    STANDARD_CONSTRUCTOR()
    if (xtbl != nullptr && xsize > 0) {   /* Copy the x breakpoints */
        xtable = new double[xsize];
        if (xtable != nullptr) {
            for (unsigned int i = 0; i < xsize; i++) xtable[i] = xtbl[i];
            nx = xsize;
            valid = isValid();
        }
    }
}

void Table1::copyData(const Table1& org, const bool cc)
{
    BaseClass::copyData(org);

    // Delete old data
    if (!cc && xtable != nullptr) { delete[] xtable; xtable = nullptr; }

    // Copy new data
    nx = org.nx;
    if (org.xtable != nullptr) {
        xtable = new double[nx];
        for (unsigned int i = 0; i < nx; i++) xtable[i] = org.xtable[i];
    }
    else xtable = nullptr;
    valid = isValid();
}

void Table1::deleteData()
{
    if (xtable != nullptr) delete[] xtable;
    xtable = nullptr;
    nx = 0;
}

//------------------------------------------------------------------------------
// Load a 1D vector with nx values.
// Example:  [ 1 2 3 ]
//------------------------------------------------------------------------------
bool Table1::loadData(const List& list, double* const table)
{
    // Make sure we have the proper number of entries in the list
    const unsigned int n1{list.entries()};
    if (n1 <= 0 || n1 != nx) return false;

    // Transfer numbers from the list to a temp table
    const auto p = new double[nx];
    const unsigned int n2{list.getNumberList(p, nx)};
    bool ok{nx == n2};
    if (ok) {
        // all is well -- copy the data
        for( unsigned int i = 0; i < nx; i++) table[i] = p[i];
        valid = isValid();
    }
    delete[] p;
    return ok;
}

//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool Table1::isValid() const
{
    return (nx >= 1) && (xtable != nullptr) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- return the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table1::tableSize() const
{
    return nx;
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty
//------------------------------------------------------------------------------
double Table1::getMinX() const
{
    if (xtable != nullptr && nx > 0)
        return (xtable[0] < xtable[nx - 1]) ? xtable[0] : xtable[nx - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

double Table1::getMaxX() const
{
    if (xtable != nullptr && nx > 0)
        return (xtable[0] < xtable[nx - 1]) ? xtable[nx - 1] : xtable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

//------------------------------------------------------------------------------
//  1D LFI
//------------------------------------------------------------------------------
double Table1::lfi(const double iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_1D(iv1, getXData(), getNumXPoints(), getDataTable(), isExtrapolationEnabled(), &s->xbp);
   } else {
      return lfi_1D(iv1, getXData(), getNumXPoints(), getDataTable(), isExtrapolationEnabled());
   }
}

//------------------------------------------------------------------------------
// setXBreakpoints1() -- for Table1
//------------------------------------------------------------------------------
bool Table1::setXBreakpoints1(const List* const sxb1obj)
{
    if (sxb1obj != nullptr) {
        loadVector(*sxb1obj, &xtable, &nx);
        valid = isValid();
    }
    return true;
}

}
}
