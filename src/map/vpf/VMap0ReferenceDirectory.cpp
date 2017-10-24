
#include "mixr/map/vpf/VMap0ReferenceDirectory.hpp"

#include "mixr/map/vpf/VpfIndexTable.hpp"
#include "mixr/map/vpf/VpfRecord.hpp"

#include "mixr/base/String.hpp"

#include <cstring>
#include <iostream>

namespace mixr {
namespace vpf {

IMPLEMENT_SUBCLASS(VMap0ReferenceDirectory, "VMap0ReferenceDirectory")
EMPTY_SLOTTABLE(VMap0ReferenceDirectory)
EMPTY_COPYDATA(VMap0ReferenceDirectory)

VMap0ReferenceDirectory::VMap0ReferenceDirectory()
{
    STANDARD_CONSTRUCTOR()
}

void VMap0ReferenceDirectory::deleteData()
{
    for (int i = 0; i < MAX_COVERAGES; i++) {
        if (coverages[i] != 0) coverages[i]->unref();
        coverages[i] = 0;
    }
}

void VMap0ReferenceDirectory::loadTables()
{
    bool ok {};
    bool cvgOk {};
    // first, open our coverage attribute table, then read back the data to us
    base::String* string {new base::String(getPath())};
    VpfTable* table {};

    // create our table
    createTable(VpfDirectory::CAT);
    table = getTable(VpfDirectory::CAT);
    if (table != nullptr) {
        ok = table->loadTableFromFile(string->getString(), "cat", VpfDirectory::CAT);
        cvgOk = ok;
    }

    // library header table
    table = nullptr;
    createTable(VpfDirectory::LHT);
    table = getTable(VpfDirectory::LHT);
    if (table != nullptr) {
        ok = table->loadTableFromFile(string->getString(), "lht", VpfDirectory::LHT);
    }

    table = nullptr;
    createTable(VpfDirectory::GRT);
    table = getTable(VpfDirectory::GRT);
    if (table != nullptr) {
        // geographic reference table
        ok = table->loadTableFromFile(string->getString(), "grt", VpfDirectory::GRT);
        if (ok) {
            // go through and read our records
            int count {1};
            VpfRecord* r {table->getRecord(count)};
            while (r != nullptr) {
                for (int i = 1; i < table->getNumberOfColumns(); i++) {
                    if (r != nullptr) {   
#if defined DEBUG_OUTPUT
                        std::cout << "COLUMN #" << i << " = " << r->getData(i) << std::endl;
#endif
                    }
                }
                r = table->getRecord(++count);
            }
        }
    }

    //table = 0;
    //createTable(VPFDirectory::DQT);
    //table = getTable(VPFDirectory::DQT);
    //if (table != 0) {
    //    // data quality table
    //    string->empty();
    //    string->catStr(getPath());
    //    string->catStr("/dqt");
    //    ok = table->loadTableFromFile(string->getString(), VPFDirectory::DQT);
    //}

    table = nullptr;
    createTable(VpfDirectory::LINEAGE);
    table = getTable(VpfDirectory::LINEAGE);
    if (table != nullptr) {
        // lineage
        ok = table->loadTableFromFile(string->getString(), "lineage.doc", VpfDirectory::LINEAGE);
    }

    // now we have all our data type and values loaded from the rference directory, we must descend into our coverage directories
    // read our coverage attribute table to determine the names of our coverages
    if (cvgOk) {
        table = nullptr;
        // our file is good, read through and open our directories (should be 4)
        table = getTable(VpfDirectory::CAT);
        if (table != nullptr) {
            // this is where we get our data
            int index {1};
            VpfRecord* record {table->getRecord(index)};
            while (record != nullptr) {
                string->empty();
                // column 2 is the column we need
                char* x = const_cast<char*>(record->getData(2));
                std::size_t size {std::strlen(x)};
                std::size_t count {};
                while (x[count] != ' ' && count < size) count++;
                x[count] = 0;

#if 1
                //if (strcmp(x, "polbnd") == 0) {
                //    if (coverages[CVG_POLBND] == 0) coverages[CVG_POLBND] = new VMAP0RefCoverageDirectory();
                //    string->catStr(getPath());
                //    string->catStr(x);
                //    string->catStr("/");
                //    // make sure you set the type first, or it will not create the proper tables
                //    coverages[CVG_POLBND]->setType(CVG_POLBND);
                //    coverages[CVG_POLBND]->setSlotPath(string);
                //}
                if (std::strcmp(x, "placenam") == 0) {
                    if (coverages[CVG_PLACENAM] == 0) coverages[CVG_PLACENAM] = new VMap0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr(x);
                    string->catStr("/");
                    // make sure you set the type first, or it will not create the proper tables
                    coverages[CVG_PLACENAM]->setType(CVG_PLACENAM);
                    coverages[CVG_PLACENAM]->setPath(string);
                }
#endif

#if 0
                if (strcmp(x, "libref") == 0) {
                    if (coverages[CVG_LIBREF] == 0) coverages[CVG_LIBREF] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_LIBREF]->setSlotPath(string);
                }
                else if (strcmp(x, "placenam") == 0) {
                    if (coverages[CVG_PLACENAM] == 0) coverages[CVG_PLACENAM] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_PLACENAM]->setSlotPath(string);
                }
                else if (strcmp(x, "dbref") == 0) {
                    if (coverages[CVG_DBREF] == 0) coverages[CVG_DBREF] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_DBREF]->setSlotPath(string);
                }
                else if (strcmp(x, "libref") == 0) {
                    if (coverages[CVG_LIBREF] == 0) coverages[CVG_LIBREF] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_LIBREF]->setSlotPath(string);
                }
                else if (strcmp(x, "polbnd") == 0) {
                    if (coverages[CVG_POLBND] == 0) coverages[CVG_POLBND] = new VMAP0RefCoverageDirectory();
                    string->catStr(getPath());
                    string->catStr("/");
                    string->catStr(x);
                    coverages[CVG_POLBND]->setSlotPath(string);
                }
#endif 
                index++;
                record = table->getRecord(index);
            }
        }
    }
    string->unref();
}

VMap0RefCoverageDirectory* VMap0ReferenceDirectory::getCoverage(const int cov)
{
    if (cov < MAX_COVERAGES) return coverages[cov];
    return nullptr;
}

}
}
