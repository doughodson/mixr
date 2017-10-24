
#include "mixr/base/functors/Table3.hpp"

#include "mixr/base/functors/TableStorage.hpp"
#include "mixr/base/util/lfi.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Table3, "Table3")

BEGIN_SLOTTABLE(Table3)
    "z",           // Z breakpoints
END_SLOTTABLE(Table3)

BEGIN_SLOT_MAP(Table3)
    ON_SLOT(1, setZBreakpoints3, List)
END_SLOT_MAP()

Table3::Table3() : Table2()
{
   STANDARD_CONSTRUCTOR()
}

Table3::Table3(const double* dtbl, const unsigned int dsize,
                   const double* xtbl, const unsigned int xsize,
                   const double* ytbl, const unsigned int ysize,
                   const double* ztbl, const unsigned int zsize)
                   : Table2(dtbl, dsize, xtbl, xsize, ytbl, ysize)
{
    STANDARD_CONSTRUCTOR()
    if (ztbl != nullptr && zsize > 0) {   // Copy the z breakpoints
        ztable = new double[zsize];
        if (ztable != nullptr) {
            for (unsigned int i = 0; i < zsize; i++) ztable[i] = ztbl[i];
            nz = zsize;
            valid = isValid();
        }
    }
}

void Table3::copyData(const Table3& org, const bool cc)
{
    BaseClass::copyData(org);

    // Delete old data
    if (!cc && ztable != nullptr) { delete[] ztable; ztable = nullptr; }

    // Copy new data
    nz = org.nz;
    if (org.ztable != nullptr) {
        ztable = new double[nz];
        for (unsigned int i = 0; i < nz; i++) ztable[i] = org.ztable[i];
    }
    else ztable = nullptr;
    valid = isValid();
}

void Table3::deleteData()
{
    if (ztable != nullptr) delete[] ztable;
    ztable = nullptr;
    nz = 0;
}

//------------------------------------------------------------------------------
// 3D table is input as a list of 2D sub-tables.
// Example:  { { [ 111 112 113 ] [ 121 122 123 ] [ 131 132 133 ] }
//             { [ 211 212 213 ] [ 221 222 223 ] [ 231 232 233 ] }
//             { [ 311 312 313 ] [ 321 322 323 ] [ 331 332 333 ] } }
//------------------------------------------------------------------------------
bool Table3::loadData(const List& list, double* const table)
{
    // Make sure we have the proper number of entries in the list
    const unsigned int n1{list.entries()};
    bool ok{n1 > 0 && n1 == nz};

    // Process each item in the list
    unsigned int i{};
    unsigned int k{BaseClass::tableSize()};
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
bool Table3::isValid() const
{
    return (nz >= 1) && (ztable != nullptr) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- return the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table3::tableSize() const
{
    return nz * BaseClass::tableSize();
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty
//------------------------------------------------------------------------------
double Table3::getMinZ() const
{
    if (ztable != nullptr && nz > 0)
        return (ztable[0] < ztable[nz - 1]) ? ztable[0] : ztable[nz - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

double Table3::getMaxZ() const
{
    if (ztable != nullptr && nz > 0)
        return (ztable[0] < ztable[nz - 1]) ? ztable[nz - 1] : ztable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

//------------------------------------------------------------------------------
//  3D LFIs
//------------------------------------------------------------------------------
double Table3::lfi(const double iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const double* y_data = getYData();
   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_3D( iv1, y_data[0], ztable[0], getXData(), getNumXPoints(),
                         y_data, getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp );
   } else {
      return lfi_3D( iv1, y_data[0], ztable[0], getXData(), getNumXPoints(),
                         y_data, getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

double Table3::lfi(const double iv1, const double iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_3D( iv1, iv2, ztable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp );
   } else {
      return lfi_3D( iv1, iv2, ztable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getDataTable(), isExtrapolationEnabled() );
   }
}

double Table3::lfi(const double iv1, const double iv2, const double iv3, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_3D( iv1, iv2, iv3, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp );
   } else {
      return lfi_3D( iv1, iv2, iv3, getXData(), getNumXPoints(), getYData(),
                         getNumYPoints(), getZData(), getNumZPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setZBreakpoints3() -- for Table3
//------------------------------------------------------------------------------
bool Table3::setZBreakpoints3(const List* const szb3obj)
{
    if (szb3obj != nullptr) {
        loadVector(*szb3obj, &ztable, &nz);
        valid = isValid();
    }
    return true;
}

}
}
