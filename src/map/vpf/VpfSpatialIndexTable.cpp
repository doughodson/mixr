
#include "mixr/map/vpf/VpfSpatialIndexTable.hpp"
#include "mixr/map/vpf/VpfDirectory.hpp"

#include "mixr/base/String.hpp"

#include <iostream>
#include <fstream>

namespace mixr {
namespace vpf {

IMPLEMENT_SUBCLASS(VpfSpatialIndexTable, "VpfSpatialIndexTable")
EMPTY_SLOTTABLE(VpfSpatialIndexTable)
EMPTY_COPYDATA(VpfSpatialIndexTable)

VpfSpatialIndexTable::VpfSpatialIndexTable()
{
    STANDARD_CONSTRUCTOR()
}

void VpfSpatialIndexTable::deleteData()
{
    if (fullPath != nullptr) {
        fullPath->unref();
        fullPath = nullptr;
    }
}

void VpfSpatialIndexTable::loadIndexTableFromFile(const char* pathname, const char* filename, const int t)
{
#if defined DEBUG_OUTPUT
    std::cout << "SPACIAL INDEX PATH AND FILE = " << pathname << filename << std::endl;
#endif
    type = t;
    fullPath = new base::String(pathname);
    fullPath->catStr(filename);
    std::ifstream inStream;
    inStream.open(fullPath->getString(), std::ios::in | std::ios::binary);
    if (inStream.fail()) std::cerr << "VpfSpatialIndexTable::loadIndexFromFile(), failed to open file " << filename << std::endl;
    else {
        inStream.seekg(0, std::ios::beg);
        // ok, our first bit should be our number of primitives
        inStream.read((char*)&numPrims, sizeof(numPrims));
        inStream.read((char*)&mbrX1, sizeof(mbrX1));
        inStream.read((char*)&mbrY1, sizeof(mbrY1));
        inStream.read((char*)&mbrX2, sizeof(mbrX2));
        inStream.read((char*)&mbrY2, sizeof(mbrY2));
        inStream.read((char*)&numNodes, sizeof(numNodes));
#if defined DEBUG_OUTPUT
        std::cout << "NUMBER OF PRIMITIVES = " << numPrims << std::endl;
        std::cout << "MBR X1 = " << mbrX1 << std::endl;
        std::cout << "MBR Y1 = " << mbrY1 << std::endl;
        std::cout << "MBR X2 = " << mbrX2 << std::endl;
        std::cout << "MBR Y2 = " << mbrY2 << std::endl;
        std::cout << "NUMBER OF NODES = " << numNodes << std::endl;
#endif

        headerByteOffset = 24;
        binSize = numNodes * 8;

        loaded = true;
        inStream.close();
    }
#if defined DEBUG_OUTPUT
    std::cout << "CLOSE FILE = " << filename << std::endl;
#endif
}

int VpfSpatialIndexTable::findPrimitivesBySpatialQuery(const float lat, const float lon, int primIds[], const float width, const float height)
{
    //std::cout << "LAT / LON = " << lat << " / " << lon << std::endl;
    int primCount {}; 
    // first, check our minimum bounding rectangle just to see if we contain this lat/lon point
    if ((lon >= mbrX1 && lon <= mbrX2) && (lat >= mbrY1 && lat <= mbrY2)) {
        //std::cout << "VALID LAT / LON convert to spatial coordinates!" << std::endl;
        int x{}, y{};
        int left{}, right{}, top{}, bottom{};
        // flag to tell our checker to check for a space query, not just a point query
        bool spaceQuery{};
        // we are going to convert our latitude to spatial coordinates, but we are also going to create spacial coordinates for our width and height
        if (width != 0 && height != 0) {
            spaceQuery = true;
            float widthDeg {(width / 60) / 2.0f};
            float heightDeg {(height / 60) / 2.0f};
            convertDegsToSpatialPoint(lat + heightDeg, lon + widthDeg, top, right);
            convertDegsToSpatialPoint(lat - heightDeg, lon - widthDeg, bottom, left);
        }

        // convert our reference lat / lon to spatial coordinates
        convertDegsToSpatialPoint(lat, lon, y, x);

#if defined DEBUG_OUTPUT
        std::cout << "Lat / Lon Spatial point = " << y << ", " << x << std::endl;
        std::cout << "TOP RIGHT Spatial point = " << top << ", " << right << std::endl;
        std::cout << "BOTTOM LEFT Spatial point = " << bottom << ", " << left << std::endl;
#endif
        // ok, we have the spatial point, now let's start reading the records that are in this coordinate
        std::fstream stream;
        stream.open(fullPath->getString(), std::ios::in | std::ios::binary);
        if (stream.fail()) std::cout << "COULD NOT OPEN FILE = " << fullPath->getString() << std::endl;
        else {
            // now get our first cell, and go from there
            for (int i = 0; i < numNodes; i++) {
                stream.seekg(headerByteOffset + (i * 8), std::ios::beg);
                int offset {}, count {};
                stream.read((char*)&offset, sizeof(offset));
                stream.read((char*)&count, sizeof(count));
                if (count == 0 && offset == 0) {
                    //std::cout << "NO PRIMITIVES FOR CELL #" << i+1 << std::endl;
                }
                else {
                    //std::cout << "CELL #" << i+1 << " OFFSET/COUNT = " << offset << " / " << count << std::endl;
                    stream.seekg(headerByteOffset + binSize + offset, std::ios::beg);
                    // now read our primitive information
                    for (int i = 0; i < count; i++) {
                        int minLon {};
                        int minLat {};
                        int maxLon {};
                        int maxLat {};
                        stream.read((char*)&minLon, 1);
                        stream.read((char*)&minLat, 1);
                        stream.read((char*)&maxLon, 1);
                        stream.read((char*)&maxLat, 1);
                        int primId {};
                        stream.read((char*)&primId, sizeof(primId));
                        if (spaceQuery) {
                            if ((left <= minLon && right >= maxLon) && (top >= maxLat && bottom <= minLat)) {
                                //std::cout << "PRIMITIVE ID OF PRIMITIVE THAT FITS QUERY = " << primId << std::endl;
                                //std::cout << "Min LONGITUDE = " << minLon << std::endl;
                                //std::cout << "Max LONGITUDE = " << maxLon << std::endl;
                                //std::cout << "Min LATITUDE = " << minLat << std::endl;
                                //std::cout << "Max LATITUDE = " << maxLat << std::endl;
                                primIds[primCount++] = primId;
                            }
                        }
                        else {
                            if ((x >= minLon && x <= maxLon) && (y >= minLat && y <= maxLat)) {
                                //std::cout << "PRIMITIVE ID OF PRIMITIVE THAT FITS QUERY = " << primId << std::endl;
                                //std::cout << "Min LONGITUDE = " << minLon << std::endl;
                                //std::cout << "Max LONGITUDE = " << maxLon << std::endl;
                                //std::cout << "Min LATITUDE = " << minLat << std::endl;
                                //std::cout << "Max LATITUDE = " << maxLat << std::endl;
                                primIds[primCount++] = primId;
                            }
                        }
                    }
                }
            }
            stream.close();       
        }
    }
    return primCount;

}

void VpfSpatialIndexTable::convertDegsToSpatialPoint(const float lat, const float lon, int& y, int& x)
{
    // we should already have checked the mbr to make sure these lat/lons are valid
    if ((mbrY2 - mbrY1) != 0) y = static_cast<int>(255 * (lat - mbrY1) / (mbrY2 - mbrY1));
    else y = 0;
    if ((mbrX2 - mbrX1) != 0) x = static_cast<int>(255 * (lon - mbrX1) / (mbrX2 - mbrX1));
    else x = 0;
    if (y > 255) y = 255;
    else if (y < 0) y = 0;
    if (x > 255) x = 255;
    else if (x < 0) x = 0;
}

}
}
