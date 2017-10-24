
#include "mixr/map/vpf/VMap0MainDirectory.hpp"

#include "mixr/map/vpf/VMap0ReferenceDirectory.hpp"
#include "mixr/map/vpf/VMap0LibDirectory.hpp"
#include "mixr/map/vpf/VpfRecord.hpp"

#include "mixr/base/String.hpp"

#include <cstring>
#include <cstddef>

namespace mixr {
namespace vpf {

IMPLEMENT_SUBCLASS(VMap0MainDirectory, "VMap0MainDirectory")
EMPTY_SLOTTABLE(VMap0MainDirectory)

VMap0MainDirectory::VMap0MainDirectory()
{
    STANDARD_CONSTRUCTOR()
}

void VMap0MainDirectory::copyData(const VMap0MainDirectory& org, const bool)
{
    BaseClass::copyData(org);
}

void VMap0MainDirectory::deleteData()
{
    if (rference != nullptr) {
        rference->unref();
        rference = nullptr;
    }
    if (libDirectory != nullptr) {
        libDirectory->unref();
        libDirectory = nullptr;
    }
}

void VMap0MainDirectory::loadTables()
{
    base::String* string {new base::String(getPath())};

    // load our database header table
    // create our table first
    createTable(VpfDirectory::DHT);
    VpfTable* table {getTable(VpfDirectory::DHT)};
    bool ok {};
    if (table != nullptr) ok = table->loadTableFromFile(string->getString(), "dht", VpfDirectory::DHT);

    // this is the main directory, which for a level 0 map, should use 
    // load our library attribute table
    table = nullptr;
    // create our table
    createTable(VpfDirectory::LAT);
    table = getTable(VpfDirectory::LAT);
    if (table != nullptr) {
        ok = table->loadTableFromFile(string->getString(), "lat", VpfDirectory::LAT);
        // now create our reference library directory
        if (ok) {
            int index {1};
            VpfRecord* record {table->getRecord(index)};
            while (record != nullptr) {
                string->empty();
                //std::cout << "LAT INFO = " << record->getData(1)
                //          << ", " << record->getData(2) << ", "
                //          << record->getData(3) << ", " << record->getData(4)
                //          << ", " << record->getData(5) << ", "
                //          << record->getData(6) << std::endl;
                // column 2 is the column we need
                char* x = const_cast<char*>(record->getData(2));
                std::size_t size {std::strlen(x)};
                std::size_t count {};
                while (x[count] != ' ' && count < size) count++;
                x[count] = 0;
                if (std::strcmp(x, "rference") == 0) {
                    string->catStr(getPath());
                    string->catStr(x);
                    string->catStr("/");
                    if (rference == nullptr) rference = new VMap0ReferenceDirectory();
                    rference->setPath(string);
                }
                //else if (strcmp(x, "noamer") == 0) {
                //    string->catStr(getPath());
                //    string->catStr("noamer");
                //    if (libDirectory == 0) libDirectory = new VMAP0LibDirectory();
                //    libDirectory->setSlotPath(string);
                //}
                index++;
                record = table->getRecord(index);
            }

        }
    }
    string->unref();
}

}
}
