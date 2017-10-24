
#include "mixr/base/functors/Table4.hpp"

#include "mixr/base/functors/TableStorage.hpp"
#include "mixr/base/util/lfi.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Table4, "Table4")

BEGIN_SLOTTABLE(Table4)
    "w",           // W breakpoints
END_SLOTTABLE(Table4)

BEGIN_SLOT_MAP(Table4)
    ON_SLOT(1, setWBreakpoints4, List)
END_SLOT_MAP()

Table4::Table4() : Table3()
{
   STANDARD_CONSTRUCTOR()
}

Table4::Table4(const double* dtbl, const unsigned int dsize,
                   const double* xtbl, const unsigned int xsize,
                   const double* ytbl, const unsigned int ysize,
                   const double* ztbl, const unsigned int zsize,
                   const double* wtbl, const unsigned int wsize)
                   : Table3(dtbl, dsize, xtbl, xsize, ytbl, ysize, ztbl, zsize)
{
    STANDARD_CONSTRUCTOR()
    if (wtbl != nullptr && wsize > 0) {   /* Copy the w breakpoints */
        wtable = new double[wsize];
        if (wtable != nullptr) {
            for (unsigned int i = 0; i < wsize; i++) wtable[i] = wtbl[i];
            nw = wsize;
            valid = isValid();
        }
    }
}

void Table4::copyData(const Table4& org, const bool cc)
{
    BaseClass::copyData(org);

    // Delete old data
    if (!cc && wtable != nullptr) { delete[] wtable; wtable = nullptr; }

    // Copy new data
    nw = org.nw;
    if (org.wtable != nullptr) {
        wtable = new double[nw];
        for (unsigned int i = 0; i < nw; i++) wtable[i] = org.wtable[i];
    }
    else wtable = nullptr;
    valid = isValid();
}

void Table4::deleteData()
{
    if (wtable != nullptr) delete[] wtable;
    wtable = nullptr;
    nw = 0;
}

//------------------------------------------------------------------------------
// 4D table is input as a list of 3D sub-tables.
// example:  { { { [ 1111 1112 1113 ] [ 1121 1122 1123 ] [ 1131 1132 1133 ] } }
//             { { [ 1211 1212 1213 ] [ 1221 1222 1223 ] [ 1231 1232 1233 ] } }
//             { { [ 1311 1312 1313 ] [ 1321 1322 1323 ] [ 1331 1332 1333 ] } }
//
//             { { [ 2111 2112 2113 ] [ 2121 2122 2123 ] [ 2131 2132 2133 ] } }
//             { { [ 2211 2212 2213 ] [ 2221 2222 2223 ] [ 2231 2232 2233 ] } }
//             { { [ 2311 2312 2313 ] [ 2321 2322 2323 ] [ 2331 2332 2333 ] } }
//
//             { { [ 3111 3112 3113 ] [ 3121 3122 3123 ] [ 3131 3132 3133 ] } }
//             { { [ 3211 3212 3213 ] [ 3221 3222 3223 ] [ 3231 3232 3233 ] } }
//             { { [ 3311 3312 3313 ] [ 3321 3322 3323 ] [ 3331 3332 3333 ] } }
//
//             { { [ 4111 4112 4113 ] [ 4121 4122 4123 ] [ 4131 4132 4133 ] } }
//             { { [ 4211 4212 4213 ] [ 4221 4222 4223 ] [ 4231 4232 4233 ] } }
//             { { [ 4311 4312 4313 ] [ 4321 4322 4323 ] [ 4331 4332 4333 ] } } }
//------------------------------------------------------------------------------
bool Table4::loadData(const List& list, double* const table)
{
    // Make sure we have the proper number of entries in the list
    const unsigned int n1{list.entries()};
    bool ok{n1 > 0 && n1 == nw};

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
bool Table4::isValid() const
{
    return (nw >= 1) && (wtable != nullptr) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// tableSize() -- return the size of the (sub)table
//------------------------------------------------------------------------------
unsigned int Table4::tableSize() const
{
    return nw * BaseClass::tableSize();
}

//------------------------------------------------------------------------------
// Minimum and maximum breakpoint functions --
//     Return the min/max values from the breakpoint tables
//     Throw an ExpInvalidTable exception if the breakpoint table is empty
//------------------------------------------------------------------------------
double Table4::getMinW() const
{
    if (wtable != nullptr && nw > 0)
        return (wtable[0] < wtable[nw - 1]) ? wtable[0] : wtable[nw - 1];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}

double Table4::getMaxW() const
{
    if (wtable != nullptr && nw > 0)
        return (wtable[0] < wtable[nw - 1]) ? wtable[nw - 1] : wtable[0];
    else
        throw new ExpInvalidTable();    //invalid table - throw an exception
}


//------------------------------------------------------------------------------
//  4D LFIs
//------------------------------------------------------------------------------
double Table4::lfi(const double iv1, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const double* y_data{getYData()};
   const double* z_data{getZData()};
   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_4D( iv1, y_data[0], z_data[0], wtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   }
   else {
      return lfi_4D( iv1, y_data[0], z_data[0], wtable[0], getXData(),
                         getNumXPoints(), y_data, getNumYPoints(), z_data,
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

double Table4::lfi(const double iv1, const double iv2, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   const double* z_data{getZData()};
   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_4D( iv1, iv2, z_data[0], wtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   } else {
      return lfi_4D( iv1, iv2, z_data[0], wtable[0], getXData(),
                         getNumXPoints(), getYData(), getNumYPoints(),
                         z_data, getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

double Table4::lfi(const double iv1, const double iv2, const double iv3, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
      const auto s = dynamic_cast<TableStorage*>(f);
      if (s == nullptr) throw new ExpInvalidFStorage();

      return lfi_4D( iv1, iv2, iv3, wtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled(),
                         &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   } else {
      return lfi_4D( iv1, iv2, iv3, wtable[0], getXData(), getNumXPoints(),
                         getYData(), getNumYPoints(), getZData(),
                         getNumZPoints(), getWData(), getNumWPoints(),
                         getDataTable(), isExtrapolationEnabled() );
   }
}

double Table4::lfi(const double iv1, const double iv2, const double iv3, const double iv4, FStorage* const f) const
{
   if (!valid) throw new ExpInvalidTable(); // Not valid - throw an exception

   if (f != nullptr) {
       const auto s = dynamic_cast<TableStorage*>(f);
       if (s == nullptr) throw new ExpInvalidFStorage();

       return lfi_4D( iv1, iv2, iv3, iv4, getXData(), getNumXPoints(),
                           getYData(), getNumYPoints(), getZData(),
                           getNumZPoints(), getWData(), getNumWPoints(),
                           getDataTable(), isExtrapolationEnabled(),
                           &s->xbp, &s->ybp, &s->zbp, &s->wbp );
   } else {
       return lfi_4D( iv1, iv2, iv3, iv4, getXData(), getNumXPoints(),
                           getYData(), getNumYPoints(), getZData(),
                           getNumZPoints(), getWData(), getNumWPoints(),
                           getDataTable(), isExtrapolationEnabled() );
   }
}

//------------------------------------------------------------------------------
// setWBreakpoints4() -- For Table4
//------------------------------------------------------------------------------
bool Table4::setWBreakpoints4(const List* const swb4obj)
{
    if (swb4obj != nullptr) {
        loadVector(*swb4obj, &wtable, &nw);
        valid = isValid();
    }
    return true;
}

}
}
