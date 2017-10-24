
#include "mixr/map/vpf/VpfDirectory.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/util/str_utils.hpp"

#include <iostream>

namespace mixr {
namespace vpf {

IMPLEMENT_SUBCLASS(VpfDirectory, "VpfDirectory")

BEGIN_SLOTTABLE(VpfDirectory)
    "path",                 // path to the vpf database directory
END_SLOTTABLE(VpfDirectory)

BEGIN_SLOT_MAP(VpfDirectory)
    ON_SLOT(1, setSlotPath, base::String)
END_SLOT_MAP()

VpfDirectory::VpfDirectory()
{
    STANDARD_CONSTRUCTOR()
}

void VpfDirectory::copyData(const VpfDirectory& org, const bool cc)
{
    BaseClass::copyData(org);
    if (!cc) {
        if (path != nullptr) delete[] path;
    }
    path = nullptr;
}

void VpfDirectory::deleteData()
{
    for (int i = 0; i < MAX_TABLES; i++) {
        if (tables[i] != nullptr) tables[i]->unref();
        tables[i] = nullptr;
    }
}

void VpfDirectory::createTable(const int x)
{
    if (x < MAX_TABLES) {
        if (tables[x] == nullptr) tables[x] = new VpfTable();
    }
}

//------------------------------------------------------------------------------
//  setPath() - sets the path to database directory directory 
//------------------------------------------------------------------------------
bool VpfDirectory::setPath(const base::String* const sfpobj)
{
    bool ok {true};
    if (sfpobj != nullptr) {
        std::size_t j {sfpobj->len()};
        if (j > 0) {
            path = new char[j+1];
            base::utStrcpy(path, j+1, *sfpobj);
        }
        else {
            std::cerr << "VPFDirectory::setPath: Invalid path value" << std::endl;
            ok = false;
        }
    }

    // ok, this is our directory, which means we should own certain table files
    loadTables();

    return ok;
}

void VpfDirectory::loadTables()
{
    // this is where our directories can load up
    // their specific tables and directories
}

}
}

