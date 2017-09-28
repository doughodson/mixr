
#ifndef __mixr_map_vpf_VpfSpatialIndexTable_H__
#define __mixr_map_vpf_VpfSpatialIndexTable_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base { class String; }
namespace vpf {

// --------------------------------------------------------------
// Class: VpfSpatialIndexTable
// Description: Spatial index file which contains the minimum bounding 
// rectangle (mbr) information about each primitive.  This allows
// for quick referencing of primitives by area
// ---------------------------------------------------------------
class VpfSpatialIndexTable : public base::Object
{
   DECLARE_SUBCLASS(VpfSpatialIndexTable, base::Object)

public:
    VpfSpatialIndexTable();

    virtual void loadIndexTableFromFile(const char* pathname, const char* filename, const int t = -1);

    bool isLoaded()                 { return loaded; }

    int findPrimitivesBySpatialQuery(const float lat, const float lon, int primIds[], const float width = 0, const float height = 0);

private:
    void convertDegsToSpatialPoint(const float lat, const float lon, int& x, int& y);
    int type {1};
    int numPrims {};
    float mbrX1 {};
    float mbrX2 {};
    float mbrY1 {};
    float mbrY2 {};
    int numNodes {};
    // Offset from the beginning of our header (24 bytes)
    int headerByteOffset {24};
    // Size of our bin (numNodes * 8 bytes)
    int binSize {};
    bool loaded {};
    base::String* fullPath {};
};

}
}

#endif
