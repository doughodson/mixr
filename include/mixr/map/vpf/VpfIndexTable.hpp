
#ifndef __mixr_map_vpf_VpfIndexTable_H__
#define __mixr_map_vpf_VpfIndexTable_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base { class String; }
namespace vpf {
class VpfDataType;

// ------------------------------------------------------------
// Class: VpfIndexTable
// Description: Associated Index table with all tables which have
// variable length text or coordinate strings
// ------------------------------------------------------------
class VpfIndexTable : public base::Object
{
   DECLARE_SUBCLASS(VpfIndexTable, base::Object)

public:
    VpfIndexTable();

    virtual void loadIndexTableFromFile(const char* pathname, const char* filename, const int t = -1);
    virtual void getRecordPosition(const int idx, int& offset, int& length);

    int getNumRecords()     { return numEntries; }
    bool isLoaded()         { return loaded; }

private:
    int type {1};
    int numEntries {};
    int numBytes {};
    int recordSize {sizeof(int) * 2};
    bool loaded {};
    base::String* fullPath {};
};

}
}

#endif
