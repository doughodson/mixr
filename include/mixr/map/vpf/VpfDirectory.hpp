
#ifndef __mixr_map_vpf_VpfDirectory_H__
#define __mixr_map_vpf_VpfDirectory_H__

#include "mixr/base/Component.hpp"
#include "mixr/map/vpf/VpfTable.hpp"

#include <array>

namespace mixr {
namespace base { class String; }
namespace vpf {
class VPFTable;

// -------------------------------------------------------------------------------
// Class: VpfDirectory
// Description: Directory object which will understand it's VPFTables
// Vector Product Format
// -------------------------------------------------------------------------------
class VpfDirectory : public base::Component
{
   DECLARE_SUBCLASS(VpfDirectory, base::Component)

public:
    VpfDirectory();

    // Vector Product Format table types (main types of tables - other tables, such as feature tables, are
    // defined at a lower level)
    enum { DHT = 0, LAT, CAT, DQT, LHT, GRT, FCS, LINEAGE, MAX_TABLES };

    virtual void loadTables();
    const char* getPath()                  { return path; }
    virtual void setType(const int x)      { type = x; }

    int getType()                          { return type; }

    // Tables
    void createTable(const int type);
    VpfTable* getTable(const int type)     { if (tables[type] != nullptr) return tables[type]; else return nullptr; }

    bool setPath(const base::String* const);

private:
    char* path {};                                // Path name to our database directory
    int type {};                                  // Type of directory we are
    std::array<VpfTable*, MAX_TABLES> tables {};  // Tables

private:
    // slot table helper methods
    bool setSlotPath(const base::String* const x)   { return setPath(x); }
};

}
}

#endif
