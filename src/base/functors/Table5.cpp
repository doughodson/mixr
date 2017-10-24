
#include "mixr/base/functors/Table5.hpp"

#include "mixr/base/functors/TableStorage.hpp"
#include "mixr/base/util/lfi.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Table5, "Table5")

BEGIN_SLOTTABLE(Table5)
    "v",           // V breakpoints
END_SLOTTABLE(Table5)

BEGIN_SLOT_MAP(Table5)
    ON_SLOT(1, setVBreakpoints5, List)
END_SLOT_MAP()

Table5::Table5() : Table4()
{
   STANDARD_CONSTRUCTOR()
   vtable = nullptr;
   nv = 0;
}

Table5::Table5(const double* dtbl, const unsigned int dsize,
                   const double* xtbl, const unsigned int xsize,
                   const double* ytbl, const unsigned int ysize,
                   const double* ztbl, const unsigned int zsize,
                   const double* wtbl, const unsigned int wsize,
                   const double* vtbl, const unsigned int vsize)
                   : Table4(dtbl, dsize, xtbl, xsize, ytbl, ysize, ztbl, zsize, wtbl, wsize),
                     vtable(nullptr), nv(0)
{
    STANDARD_CONSTRUCTOR()
    if (vtbl != nullptr && vsize > 0) {   /* Copy the v breakpoints */
        vtable = new double[vsize];
        if (vtable != nullptr) {
            for (unsigned int i = 0; i < vsize; i++) vtable[i] = vtbl[i];
            nv = vsize;
            valid = isValid();
        }
    }
}

void Table5::copyData(const Table5& org, const bool cc)
{
    BaseClass::copyData(org);

    // Delete old data
    if (!cc && vtable != nullptr) { delete[] vtable; vtable = nullptr; }

    // Copy new data
    nv = org.nv;
    if (org.vtable != nullptr) {
        vtable = new double[nv];
        for (unsigned int i = 0; i < nv; i++) vtable[i] = org.vtable[i];
    }
    else vtable = nullptr;
    valid = isValid();
}

void Table5::deleteData()
{
    if (vtable != nullptr) delete[] vtable;
    vtable = nullptr;
    nv = 0;
}

//------------------------------------------------------------------------------
// 5D table is input as a list of 4D sub-tables.
//------------------------------------------------------------------------------
bool Table5::loadData(const List& list, double* const table)
{
    // Make sure we have the proper number of entries in the list
    const unsigned int n1{list.entries()};
    bool ok{n1 > 0 && n1 == nv};

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
bool Table5::isValid() const
{
    return (nv >= 1) && (vtable != nullptr) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- return the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table5::tableSize() const
{
    return nv * BaseClass::tableSize();
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty
//------------------------------------------------------------------------------
double Table5::getMinV() const
{
    if (vtable != nullptr && nv > 0)
        return (vtable[0] < vtable[nv - 1]) ? vtable[0] : vtable[nv - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

double Table5::getMaxV() const
{
    if (vtable != nullptr && nv > 0)
        return (vtable[0] < vtable[nv - 1]) ? vtable[nv - 1] : vtable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

//------------------------------------------------------------------------------
//  5D LFIs
//------------------------------------------------------------------------------
double Table5::lfi(const double iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const double* y_data{getYData()};
   const double* z_data{getZData()};
   const double* w_data{getWData()};
   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_5D( iv1, y_data[0], z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   } else {
      return lfi_5D( iv1, y_data[0], z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled());
   }
}

double Table5::lfi(const double iv1, const double iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const double* z_data{getZData()};
   const double* w_data{getWData()};
   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_5D( iv1, iv2, z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   } else {
      return lfi_5D( iv1, iv2, z_data[0], w_data[0], vtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled());
   }
}

double Table5::lfi(const double iv1, const double iv2, const double iv3, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const double* w_data{getWData()};
   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_5D( iv1, iv2, iv3, w_data[0], vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   } else {
      return lfi_5D( iv1, iv2, iv3, w_data[0], vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled());
   }
}

double Table5::lfi(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_5D( iv1, iv2, iv3, iv4, vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   } else {
      return lfi_5D( iv1, iv2, iv3, iv4, vtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

double Table5::lfi(const double iv1, const double iv2, const double iv3, const double iv4, const double iv5, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_5D( iv1, iv2, iv3, iv4, iv5, getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp, &s->vbp );
   } else {
      return lfi_5D( iv1, iv2, iv3, iv4, iv5, getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getVData(), getNumVPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setVBreakpoints5() -- For Table5
//------------------------------------------------------------------------------
bool Table5::setVBreakpoints5(const List* const swb5obj)
{
    if (swb5obj != nullptr) {
        loadVector(*swb5obj, &vtable, &nv);
        valid = isValid();
    }
    return true;
}

}
}
