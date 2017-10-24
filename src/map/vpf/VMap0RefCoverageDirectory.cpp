
#include "mixr/map/vpf/VMap0RefCoverageDirectory.hpp"

#include "mixr/map/vpf/VMap0FeatureClass.hpp"
#include "mixr/map/vpf/VMap0ReferenceDirectory.hpp"
#include "mixr/map/vpf/VpfTable.hpp"
#include "mixr/map/vpf/VpfSpatialIndexTable.hpp"
#include "mixr/map/vpf/VpfRecord.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/osg/Vec3d"

#include <cstring>
#include <iostream>
#include <cmath>

namespace mixr {
namespace vpf {

IMPLEMENT_SUBCLASS(VMap0RefCoverageDirectory, "VMap0RefCoverageDirectory")
EMPTY_SLOTTABLE(VMap0RefCoverageDirectory)
EMPTY_COPYDATA(VMap0RefCoverageDirectory)

VMap0RefCoverageDirectory::VMap0RefCoverageDirectory()
{
    STANDARD_CONSTRUCTOR()
}

void VMap0RefCoverageDirectory::deleteData()
{
    for (int i = 0; i < MAX_FEATURE_CLASSES; i++) {
        if (featureClass[i] != 0) featureClass[i]->unref();
        featureClass[i] = 0;
    }
    for (int i = 0; i < MAX_FEATURE_TABLES; i++) {
        if (featureClass[i] != 0) featureTables[i]->unref();
        featureTables[i] = 0;
    }
    for (int i = 0; i < MAX_SPATIAL_INDEX_TABLES; i++) {
        if (featureClass[i] != 0) spatialTables[i]->unref();
        spatialTables[i] = 0;
    }
}

void VMap0RefCoverageDirectory::loadTables()
{
    base::String* string {new base::String(getPath())};

    createTable(VpfDirectory::FCS);
    VpfTable* fcs {getTable(VpfDirectory::FCS)};
    if (fcs != nullptr && !fcs->isLoaded()) fcs->loadTableFromFile(string->getString(), "fcs", VpfDirectory::FCS);

    // ok, let's build our feature class(es) from our feature class schema
    if (fcs->isLoaded()) buildFeatureClasses();

    // based on the produt specification for VMAP level 0, there are 4 reference coverage directories
    // libref, dbref, polbnd, and placenam.
    if (getType() == VMap0ReferenceDirectory::CVG_PLACENAM) {
        // for the placenam directory, there is one simple primitive table and attribute table
        // they are placenam.pft and end
        // There is also a node spacial index (nsi) table for minimum boundary rectangle lookups
        if (featureTables[PLACENAM_DOT_PFT] == nullptr) featureTables[PLACENAM_DOT_PFT] = new VpfTable();
        if (!featureTables[PLACENAM_DOT_PFT]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[PLACENAM_DOT_PFT]->loadTableFromFile(string->getString(), "placenam.pft");
        }
        
        // ok, now our entity node primitive table
        if (featureTables[END] == nullptr) featureTables[END] = new VpfTable();
        if (!featureTables[END]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[END]->loadTableFromFile(string->getString(), "end");
        }

        // SPATIAL TABLES FOR QUICK REFERENCE!
        if (spatialTables[NSI] == nullptr) spatialTables[END] = new VpfSpatialIndexTable();
        if (!spatialTables[NSI]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            spatialTables[NSI]->loadIndexTableFromFile(string->getString(), "nsi");
            // SLS test
            // now do a quick spatial query 
            //spatialTables[NSI]->findPrimitivesBySpatialQuery(32, -115);
        }
        

    }
    else if (getType() == VMap0ReferenceDirectory::CVG_POLBND) {
        if (featureTables[CND] == nullptr) featureTables[CND] = new VpfTable();
        if (!featureTables[CND]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[CND]->loadTableFromFile(string->getString(), "cnd");
        }
        // we now have to open our edge primitive table, which will contain information
        // about how our connected nodes are put together
        if (featureTables[EDG] == nullptr) featureTables[EDG] = new VpfTable();
        if (!featureTables[EDG]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[EDG]->loadTableFromFile(string->getString(), "edg");
        }
        // now we have to open our edge bouding rectangle, because it contains information
        // to do quick queries on our edg file
        if (featureTables[EBR] == nullptr) featureTables[EBR] = new VpfTable();
        if (!featureTables[EBR]->isLoaded()) {
            // don't give it a type, because we are keeping track of the type
            featureTables[EBR]->loadTableFromFile(string->getString(), "ebr");
        }
    }
    string->unref();
}

int VMap0RefCoverageDirectory::getPolBndCoordsByLatLon(const float maxLat, const float maxLon, const float minLat, const float minLon, const int idx, base::Vec3d vec[], const int max) 
{
    int numCoords {};
    if (featureTables[EBR]->isLoaded() && featureTables[EDG]->isLoaded()) {
        int index {1};
        VpfRecord* record = featureTables[EBR]->getRecord(index);
        double xMin{}, yMin{}, xMax{}, yMax{};
        // take the absolute value of xmin thru xmax and our lat coverage, so we don't have to worry about sign
        while (record != nullptr) {
            xMin = std::atof(record->getData(2));
            xMax = std::atof(record->getData(4));
            yMin = std::atof(record->getData(3));
            yMax = std::atof(record->getData(5));
            if ((yMax <= maxLat && yMax >= minLat) || (yMin >= minLat && yMin <= maxLat)) {
                // ok, we are in our lat range, now lets check our lon!
                if ((xMax <= maxLon && xMax >= minLon) || (xMin >= minLon && xMin <= maxLon)) {
                    //int temp = record->getCoordinate
                    VpfRecord* r {featureTables[EDG]->getRecord(index)};
                    if (r != nullptr) {
                        const int temp {r->getCoordinate(8, vec, idx, max)};
                        numCoords += temp;
                    }
                }
            }                        
            index++;
            record = featureTables[EBR]->getRecord(index);
         }

#if defined DEBUG_OUTPUT
        std::cout << "NUM RECORDS = " << index << std::endl;
#endif

    }
    return numCoords;
}

int VMap0RefCoverageDirectory::getPlacenameCoordsByRecord(const int r, base::Vec3d vec[], const int idx, const int max)
{
    int numCoords {};
    // first, make sure our table exists, and then get the record
    if (featureTables[END] != nullptr && featureTables[END]->isLoaded()) {
        VpfRecord* rec {featureTables[END]->getRecord(r)};
        // our coordinate is column 3
        if (rec != nullptr) {
            numCoords = rec->getCoordinate(3, vec, idx, max);
        }
    }
    else std::cout << "NO ENTITY NODE PRIMITIVE TABLE FOUND, PATH = " << getPath() << std::endl;

    return numCoords;
}

int VMap0RefCoverageDirectory::getPlacenameCoordsByRange(const float width, const float height, const float refLat, const float refLon, base::Vec3d vec[], const int idx, const int max)
{
    int numCoords {idx};
    int tempIdx {idx};
    // get our node spatial index to do a query
    if (spatialTables[NSI] != nullptr) {
        int numPrims {spatialTables[NSI]->findPrimitivesBySpatialQuery(refLat, refLon, spatialQueryPrimIds.data(), width, height)};
        VpfRecord* v {};
        for (int i = 0; i < numPrims && (i < max-1); i++) {
            // now that we have the primitive ids of the values, we can query our entity node primitive table to obtain the coordinates
            if (featureTables[END] != nullptr) {
                v = featureTables[END]->getRecord(spatialQueryPrimIds[i]);
                if (v != nullptr && numCoords < max) {
                    int temp {v->getCoordinate(3, vec, tempIdx, max)};
                    numCoords += temp;
                    tempIdx += temp;
                }
            }
        }
    }
    return numCoords;
}

int VMap0RefCoverageDirectory::getSpatialQueryPlacenamePrimID(const int idx)
{
    if (idx < 500) return spatialQueryPrimIds[idx-1];
    return -1;
}

bool VMap0RefCoverageDirectory::getPlacenameByRecord(const int r, char* p)
{
    bool ok {};
    // first, make sure our table exists, and then get the record
    if (featureTables[PLACENAM_DOT_PFT] != 0 && featureTables[PLACENAM_DOT_PFT]->isLoaded()) {
        VpfRecord* rec {featureTables[PLACENAM_DOT_PFT]->getRecord(r)};
        // our placaname is column 2
        if (rec != nullptr) {
            ok = true;
            //lcStrcpy(p, sizeof(p), rec->getData(2));
            std::strcpy(p, rec->getData(2));
            //std::sprintf(p, "%s", rec->getData(2));
        }
    }
    else std::cout << "NO PLACENAME POINT FEATURE TABLE FOUND, PATH = " << getPath() << std::endl;
    return ok;
}

int VMap0RefCoverageDirectory::getPolBndCoordsByRecord(const int r, base::Vec3d vec[], const int idx, const int max)
{
    int numCoords {};
    // first, make sure our table exists, and then get the record
    if (featureTables[EDG] != nullptr && featureTables[EDG]->isLoaded()) {
        VpfRecord* rec {featureTables[EDG]->getRecord(r)};
        // our boundary coordinate is column 8
        if (rec != nullptr) numCoords = rec->getCoordinate(8, vec, idx, max);            
    }
    else std::cout << "NO EDGE TABLE FOUND, PATH = " << getPath() << std::endl;
    return numCoords;
}

void VMap0RefCoverageDirectory::buildFeatureClasses()
{
    char* d {};
    bool exists {};
    int holder {};
    VpfTable* fcs {getTable(VpfDirectory::FCS)};
    // first, build our classes, then add the relationships
    if (fcs != nullptr) {
        VpfRecord* record {};
        for (int i = 1; fcs->getRecord(i) != 0 && numFeatureClasses < MAX_FEATURE_CLASSES; i++) {
            record = fcs->getRecord(i);
            d = (char*)record->getData(2); 
            // now count our table name and truncate it
            std::size_t size {strlen(d)};
            std::size_t count {};
            while (d[count] != ' ' && count < size) count++;
            d[count] = 0;            

            exists = false;
            holder = 0;
            if (d != nullptr) {
                if (numFeatureClasses > 0) {
                    for (int x = 0; x < numFeatureClasses; x++) {
                        if (std::strcmp(featureClass[x]->getName(), d) == 0) {
                            holder = x;
                            exists = true;
                        }
                    }
                    if (!exists) {
                        featureClass[numFeatureClasses] = new VMap0FeatureClass();
                        featureClass[numFeatureClasses]->setName(d);
                        // add our relationship
                        featureClass[numFeatureClasses]->addRelation(record->getData(3), record->getData(5),
                                                                     record->getData(4), record->getData(6));
                        numFeatureClasses++;
                    }
                    else {
                        featureClass[holder]->addRelation(record->getData(3), record->getData(5),
                                                          record->getData(4), record->getData(6));
                    }
                }
                else {
                    featureClass[numFeatureClasses] = new VMap0FeatureClass();
                    featureClass[numFeatureClasses]->setName(d);
                    featureClass[numFeatureClasses]->addRelation(record->getData(3), record->getData(5),
                                                                 record->getData(4), record->getData(6));
                    numFeatureClasses++;
                }
            }
        }
    }
}

VMap0FeatureClass* VMap0RefCoverageDirectory::getFeatureClassByName(const char* name)
{
    for (int i = 0; i < numFeatureClasses; i++) {
        if (std::strcmp(featureClass[i]->getName(), name) == 0) return featureClass[i];
    }
    return nullptr;
}

}
}
