
#include "mixr/map/vpf/VMap0LibDirectory.hpp"

#include "mixr/map/vpf/VMap0RefCoverageDirectory.hpp"

#include "mixr/base/String.hpp"

namespace mixr {
namespace vpf {

IMPLEMENT_SUBCLASS(VMap0LibDirectory, "VMap0LibDirectory")
EMPTY_SLOTTABLE(VMap0LibDirectory)

VMap0LibDirectory::VMap0LibDirectory()
{
    STANDARD_CONSTRUCTOR()
}

void VMap0LibDirectory::copyData(const VMap0LibDirectory& org, const bool)
{
    BaseClass::copyData(org);
}

void VMap0LibDirectory::deleteData()
{
    for (int i = 0; i < MAX_COVERAGES; i++) {
        if (coverages[i] != 0) {
            coverages[i]->unref();
            coverages[i] = 0;
        }
    }
}

void VMap0LibDirectory::loadTables()
{
    // load up our tables that we own
    bool ok {};
    bool cvgOk {};
    // first, open our coverage attribute table, then read back the data to us
    base::String* string {new base::String(getPath())};
    createTable(VpfDirectory::CAT);
    VpfTable* table {getTable(VpfDirectory::CAT)};
    if (table != nullptr) {
        ok = table->loadTableFromFile(string->getString(), "cat", VpfDirectory::CAT);
        cvgOk = ok;
        #if 0
        if (ok) {
            std::cout << "Coverage Attribute Table: " << std::endl;
            for (int i = 0; i < tables[VPFDirectory::CAT].getNumberOfRows(); i++) {
                for (int j = 0; j < tables[VPFDirectory::CAT].getNumberOfColumns(); j++) {
                    std::cout << "ROW, COLUMN, VALUE = " << i+1 << ", " << j+1 << ", " << tables[VPFDirectory::CAT].getTableValue(i+1, j+1)->getValue() << std::endl;
                }
            }
        }
        #endif
    }

    // library header table
    table = nullptr;
    createTable(VpfDirectory::LHT);
    table = getTable(VpfDirectory::LHT);
    if (table != nullptr) {
        ok = table->loadTableFromFile(string->getString(), "lat", VpfDirectory::LHT);
    }

    table = nullptr;
    createTable(VpfDirectory::GRT);
    table = getTable(VpfDirectory::GRT);
    if (table != nullptr) {
        // geographic reference table
        ok = table->loadTableFromFile(string->getString(), "grt", VpfDirectory::GRT);
    }

    #if 0
    table = getTable(VPFDirectory::DQX);
    if (table != 0) {
        // data quality index
        string.empty();
        string.catStr(getPath());
        string.catStr("/dqx");
        ok = table->loadTableFromFile(string.getString(), VPFDirectory::DQX);
        if (ok) {
            std::cout << "DATA QUALITY INDEX:" << std::endl;
            for (int i = 0; i < tables[VPFDirectory::DQX].getNumberOfRows(); i++) {
                for (int j = 0; j < tables[VPFDirectory::DQX].getNumberOfColumns(); j++) {
                    std::cout << "ROW # " << i+1 << ", COLUMN # " << j+1 << " VALUE = " << tables[VPFDirectory::DQX].getTableValue(i+1, j+1)->getValue() << std::endl;
                }
            }
        }
    }
    #endif

    #if 0
    table = getTable(VPFDirectory::DQT);
    if (table != 0) {
        // data quality table
        string.empty();
        string.catStr(getPath());
        string.catStr("/dqt");
        ok = table->loadTableFromFile(string.getString(), VPFDirectory::DQT);
        if (ok) {
            std::cout << "DATA QUALITY TABLE:" << std::endl;
            for (int i = 0; i < tables[VPFDirectory::DQT].getNumberOfRows(); i++) {
                for (int j = 0; j < tables[VPFDirectory::DQT].getNumberOfColumns(); j++) {
                    std::cout << "ROW # " << i+1 << ", COLUMN # " << j+1 << " VALUE = " << tables[VPFDirectory::DQT].getTableValue(i+1, j+1)->getValue() << std::endl;
                }
            }
        }
    }
    #endif

    //table = 0;
    //createTable(VPFDirectory::LINEAGE);
    //table = getTable(VPFDirectory::LINEAGE);
    //if (table != 0) {
    //    // lineage
    //    string->empty();
    //    string->catStr(getPath());
    //    string->catStr("/lineage.doc");
    //    ok = table->loadTableFromFile(string->getString(), VPFDirectory::LINEAGE);
    //}

    // now we have all our data type and values loaded from the reference directory,
    // we must descend into our coverage directories
    // read our coverage attribute table to determine the names of our coverages
    if (cvgOk) {
        table = 0;
        // our file is good, read through and open our directories
        table = getTable(VpfDirectory::CAT);
        //VPFDataType* t = 0;
//        size_t size = 0;
//        size_t count = 0;
//        for (int i = 0; i < table->getNumberOfRows() && i < MAX_COVERAGES; i++) {
//            count = 0;
//            size = 0;
            // our value for the library comes in as an 8 character name with
            // trailing spaces, so we need to truncate it to a * character 
            // text value with no trailing spaces to put it into our pathname
            //t = table->getTableValue(i+1, 2);
            //t = 0;
            //if (t != 0 && numCoverages < MAX_COVERAGES)  {
            //    string->empty();
            //    char* val = (char*)table->getTableValue(i+1, 2)->getValue();
            //    size = strlen(val);
            //    // make sure we terminate at the correct area
            //    while (val[count] != ' ' && count < size) count++;
            //    val[count] = NULL;
            //    string->catStr(getPath());
            //    string->catStr("/");
            //    string->catStr(val);
            //    if (coverages[numCoverages] == 0) coverages[numCoverages] = new VMAP0RefCoverageDirectory();
            //    coverages[numCoverages]->setSlotPath(string);
            //    numCoverages++;
            //}
//        }
    }
}

}
}
