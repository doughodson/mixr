
#include "mixr/base/functors/Table2.hpp"

#include "mixr/base/functors/TableStorage.hpp"
#include "mixr/base/util/lfi.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Table2, "Table2")

BEGIN_SLOTTABLE(Table2)
    "y",           // Y breakpoints
END_SLOTTABLE(Table2)

BEGIN_SLOT_MAP(Table2)
    ON_SLOT(1, setYBreakpoints2,List)
END_SLOT_MAP()

Table2::Table2() : Table1()
{
   STANDARD_CONSTRUCTOR()
}

Table2::Table2(const double* dtbl, const unsigned int dsize,
                   const double* xtbl, const unsigned int xsize,
                   const double* ytbl, const unsigned int ysize)
                   : Table1(dtbl, dsize, xtbl, xsize)
{
    STANDARD_CONSTRUCTOR()
    if (ytbl != nullptr && ysize > 0) {   /* Copy the y breakpoints */
        ytable = new double[ysize];
        if (ytable != nullptr) {
            for (unsigned int i = 0; i < ysize; i++) ytable[i] = ytbl[i];
            ny = ysize;
            valid = isValid();
        }
    }
}

void Table2::copyData(const Table2& org, const bool cc)
{
    BaseClass::copyData(org);

    // Delete old data
    if (!cc && ytable != nullptr) { delete[] ytable; ytable = nullptr; }

    // Copy new data
    ny = org.ny;
    if (org.ytable != nullptr) {
        ytable = new double[ny];
        for (unsigned int i = 0; i < ny; i++) ytable[i] = org.ytable[i];
    }
    else ytable = nullptr;
    valid = isValid();
}

void Table2::deleteData()
{
    if (ytable != nullptr) delete[] ytable;
    ytable = nullptr;
    ny = 0;
}

//------------------------------------------------------------------------------
// 2D table is input as a list of 1D vectors.
// Example:  { [ 11 12 13 ] [ 21 22 23 ] [ 31 32 33 ] }
//------------------------------------------------------------------------------
bool Table2::loadData(const List& list, double* const table)
{
    // Make sure we have the proper number of entries in the list
    const unsigned int n1{list.entries()};
    bool ok{n1 > 0 && n1 == ny};

    // Process each item in the list
    unsigned int i{};
    const unsigned int k{BaseClass::tableSize()};
    const List::Item* item{list.getFirstItem()};
    while (ok && item != nullptr) {
        const auto p = dynamic_cast<const Pair*>(item->getValue());
        if (p != nullptr) {
            const auto slist = dynamic_cast<const List*>(p->object());
            if (slist != nullptr) {
                ok &= BaseClass::loadData(*slist, &table[i]);
                i += k;
            }
        }
        item = item->getNext();
    }
    if (ok) valid = isValid();
    return ok;
}

//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool Table2::isValid() const
{
    return (ny >= 1) && (ytable != nullptr) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- returns the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table2::tableSize() const
{
    return ny * BaseClass::tableSize();
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty
//------------------------------------------------------------------------------
double Table2::getMinY() const
{
    if (ytable != nullptr && ny > 0)
        return (ytable[0] < ytable[ny - 1]) ? ytable[0] : ytable[ny - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

double Table2::getMaxY() const
{
    if (ytable != nullptr && ny > 0)
        return (ytable[0] < ytable[ny - 1]) ? ytable[ny - 1] : ytable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

//------------------------------------------------------------------------------
//  2D LFIs
//------------------------------------------------------------------------------
double Table2::lfi(const double iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_2D( iv1, ytable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled(),
                         &s->xbp, &s->ybp );
   } else {
      return lfi_2D( iv1, ytable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled() );
   }
}

double Table2::lfi(const double iv1, const double iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_2D( iv1, iv2, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled(),
                         &s->xbp, &s->ybp );
   } else {
      return lfi_2D( iv1, iv2, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getDataTable(),
                         isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setYBreakpoints2() -- for Table2
//------------------------------------------------------------------------------
bool Table2::setYBreakpoints2(const List* const syb2obj)
{
    if (syb2obj != nullptr) {
        loadVector(*syb2obj, &ytable, &ny);
        valid = isValid();
    }
    return true;
}

}
}
