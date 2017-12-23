
#ifndef __mixr_map_vpf_VMap0RefCoverageDirectory_H__
#define __mixr_map_vpf_VMap0RefCoverageDirectory_H__

#include "mixr/map/vpf/VpfDirectory.hpp"

#include <array>

namespace mixr {
namespace base { class Vec3d; }
namespace vpf {
class VpfTable;
class VpfSpatialIndexTable;
class VMap0FeatureClass;

// -------------------------------------------------------------------------------
// Class: VMap0RefCoverageDirectory
// Description: VMAP level 0 coverage directory(ies), which contain
// - fcs (feature class schema)
// - libref.lft 
// - libref.tft
// - primitive tables
// -------------------------------------------------------------------------------
class VMap0RefCoverageDirectory : public VpfDirectory
{
   DECLARE_SUBCLASS(VMap0RefCoverageDirectory, VpfDirectory)

public:
    VMap0RefCoverageDirectory();

    void loadTables() override;

    virtual void buildFeatureClasses();

    VMap0FeatureClass* getFeatureClassByName(const char* name);

    // Here are all the feature table file types (attribute and primitive)
    enum { END = 0, PLACENAM_DOT_PFT, CND, EDG, EBR, MAX_FEATURE_TABLES };

    // Creates all the needed tables based on our coverage type
    void createTables();

    int getPlacenameCoordsByRecord(const int r, base::Vec3d vec[], const int idx, const int max);
    bool getPlacenameByRecord(const int r, char* x);
    
    int getPlacenameCoordsByRange(const float width, const float height, const float refLat, const float refLon, base::Vec3d vec[], const int idx, const int max);

    int getPolBndCoordsByRecord(const int r, base::Vec3d vec[], const int idx, const int max);

    // Get by lat lon bounding box
    int getPolBndCoordsByLatLon(const float maxLat, const float maxLon, const float minLat, const float minLon, const int idx, base::Vec3d vec[], const int max);

    // Here are all the spacial index table file types
    enum { NSI = 0, MAX_SPATIAL_INDEX_TABLES };

    int getSpatialQueryPlacenamePrimID(const int idx);

private:
    static const int MAX_FEATURE_CLASSES {5};
    // Our feature class schema coverage directory
    std::array<VMap0FeatureClass*, MAX_FEATURE_CLASSES> featureClass {};
    int numFeatureClasses {};                      // Number of feature classes we have

    std::array<int, 500> spatialQueryPrimIds {};   // Holds the primitive IDs of the last spatial query for quick access

    std::array<VpfTable*, MAX_FEATURE_TABLES> featureTables {};
    std::array<VpfSpatialIndexTable*, MAX_SPATIAL_INDEX_TABLES> spatialTables {};
};

}
}

#endif
