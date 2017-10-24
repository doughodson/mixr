
#ifndef __mixr_map_vpf_VpfRecord_H__
#define __mixr_map_vpf_VpfRecord_H__

#include "mixr/base/Object.hpp"

#include "mixr/map/vpf/VpfTable.hpp"

#include <array>

namespace mixr {
namespace base { class String; class Vec3d; }
namespace vpf {
class VpfTable;
class VpfDataType;

// -------------------------------------------------------------------------------
// Class: VpfRecord
// Description: This is a single fixed length or variable length object that
// will store a list of VPFDataType values for quick access.
// -------------------------------------------------------------------------------
class VpfRecord : public base::Object
{
    DECLARE_SUBCLASS(VpfRecord, base::Object)

public:
    VpfRecord();

    static const int MAX_COORDS {500};

    virtual void createRecord(VpfTable* x, const char* file, const int idx);
    
    const char* getData(const int column);

    int getCoordinate(const int column, base::Vec3d vec[], const int idx, const int max);

    bool isEOR()                { return eor; }

private:
    VpfTable* parent {};                          // Our parent table (for header information and column information)
    base::String* filename {};                    // Name of the file we are going to access
    std::array<VpfDataType*, VpfTable::MAX_COLUMNS> data {}; // Data type
    int index {};                                 // Our index number, which we will need for accessing data
    bool eor {};                                  // End of record
    int numCoords {1};                            // Number of coordinates we have
};

}
}

#endif
