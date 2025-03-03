
#include "mixr/base/relations/ITable.hpp"

#include "mixr/base/relations/FStorage.hpp"
#include "mixr/base/relations/TableStorage.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/List.hpp"

#include <iostream>

namespace mixr {
namespace base {

IMPLEMENT_PARTIAL_SUBCLASS(ITable, "ITable")

BEGIN_SLOTTABLE(ITable)
    "data",          // Data table
    "extrapolate",   // Extrapolate beyond data
END_SLOTTABLE(ITable)

BEGIN_SLOT_MAP(ITable)
    ON_SLOT(1, setSlotDataTable, List)
    ON_SLOT(2, setSlotExtrapolationEnabled, Boolean)
END_SLOT_MAP()

ITable::ITable()
{
   STANDARD_CONSTRUCTOR()
}

ITable::ITable(const double* dtbl, const unsigned int dsize)
{
    STANDARD_CONSTRUCTOR()
    if (dtbl != nullptr && dsize > 0) {   // copy the data table
        dtable = new double[dsize];
        if (dtable != nullptr) {
            for (unsigned int i = 0; i < dsize; i++) {
                dtable[i] = dtbl[i];
            }
            nd = dsize;
        }
    }
}

ITable::ITable(const ITable& org) : valid(false), extFlg(false)
{
    STANDARD_CONSTRUCTOR()
    dtable = nullptr;
    nd = 0;
    copyData(org,true);
}

ITable::~ITable()
{
   STANDARD_DESTRUCTOR()
}

ITable& ITable::operator=(const ITable& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

ITable* ITable::clone() const
{
    return nullptr;
}

void ITable::copyData(const ITable& org, const bool cc)
{
    BaseClass::copyData(org);

    // Delete old data
    if ( !cc && dtable != nullptr ) {
        delete[] dtable;
        dtable = nullptr;
    }

    // Copy new data
    nd = org.nd;
    if (org.dtable != nullptr) {
        dtable = new double[nd];
        for (unsigned int i = 0; i < nd; i++) {
            dtable[i] = org.dtable[i];
        }
    } else {
        dtable = nullptr;
    }
    valid = org.valid;
    extFlg = org.extFlg;
}

void ITable::deleteData()
{
    if (dtable != nullptr) delete[] dtable;
    dtable = nullptr;
    nd = 0;
}

//------------------------------------------------------------------------------
// isValid() -- Returns true if the data table and breakpoint tables are valid.
//------------------------------------------------------------------------------
bool ITable::isValid() const
{
   return (nd >= 1) && (dtable != nullptr) && (tableSize() == nd) && BaseClass::isValid();
}

//------------------------------------------------------------------------------
// Storage factory
//------------------------------------------------------------------------------
FStorage* ITable::storageFactory() const
{
   return new TableStorage();
}

//------------------------------------------------------------------------------
// setExtrapolationEnabled() -- set the extrapolation enabled flag
//------------------------------------------------------------------------------
bool ITable::setExtrapolationEnabled(const bool flg)
{
   extFlg = flg;
   return true;
}

bool ITable::setSlotExtrapolationEnabled(const Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setExtrapolationEnabled( msg->asBool() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// findMinMax() -- find the minimum and maximum values of the table
//------------------------------------------------------------------------------
void ITable::findMinMax(double* minValue, double* maxValue) const
{
    if (nd > 0) {
        double minv {dtable[0]};
        double maxv {dtable[0]};
        for (unsigned int i = 1; i < nd; i++) {
            if (dtable[i] < minv) minv = dtable[i];
            if (dtable[i] > maxv) maxv = dtable[i];
        }
        *minValue = minv;
        *maxValue = maxv;
    }
}


//------------------------------------------------------------------------------
// loadVector() --
//------------------------------------------------------------------------------
bool ITable::loadVector(const List& list, double** table, unsigned int* nn)
{
    const std::size_t n{list.entries()};
    if (n <= 0) return false;

    const auto p = new double[n];
    const std::size_t n2{list.getNumberList(p, n)};
    bool ok{(n == n2)};
    if (ok) {
        // Have the data!
        *table = p;
        *nn = n;
    } else {
        // Something is wrong, free the table
        delete[] p;
        throw new ExpInvalidVector();     // invalid vector - throw an exception
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setDataTable() -- for Table
//------------------------------------------------------------------------------
bool ITable::setDataTable(const List* const sdtobj)
{
    bool ok {true};
    if (sdtobj != nullptr) {
        // First determine the size of the table -- ALL breakpoint data MUST
        // have been set first (order in input file) to determine the size
        // of the data table
        const unsigned int ts {tableSize()};
        if (ts > 0) {
            // Allocate table space and load the table
            const auto p = new double[ts];
            ok = loadData(*sdtobj, p);
            if (ok) {
                // Loading completed, so
                // free up any old data and set to the new.
                if (dtable != nullptr) delete[] dtable;
                dtable = p;
                nd = ts;
            } else {
                // Something is wrong!
                delete[] p;
                std::cerr << "Table::setDataTable: Something is wrong!  Data table aborted." << std::endl;
                ok = false;
            }
        }
        valid = isValid();
    }
    return ok;
}

}
}
