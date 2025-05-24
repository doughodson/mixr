
#include "mixr/map/rpf/CadrgMap.hpp"
#include "mixr/map/rpf/CadrgFile.hpp"
#include "mixr/map/rpf/CadrgFrame.hpp"
#include "mixr/map/rpf/CadrgTocEntry.hpp"
#include "mixr/map/rpf/TexturePager.hpp"
#include "mixr/map/rpf/MapDrawer.hpp"
#include "mixr/graphics/Texture.hpp"

#include "mixr/base/numeric/Integer.hpp"

#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/String.hpp"

#include <cstring>

namespace mixr {
namespace rpf {

IMPLEMENT_SUBCLASS(CadrgMap, "CadrgMap")

BEGIN_SLOTTABLE(CadrgMap)
    "pathNames",        // Path names to our TOC file
    "maxTableSize",     // Max table size to set up
    "mapLevel",         // Map level we are going to set (if it exists)
END_SLOTTABLE(CadrgMap)

BEGIN_SLOT_MAP(CadrgMap)
    ON_SLOT(1, setSlotPathnames,    base::PairStream)
    ON_SLOT(2, setSlotMaxTableSize, base::Integer)
    ON_SLOT(3, setSlotMapLevel,     base::String)
END_SLOT_MAP()

CadrgMap::CadrgMap()
{
    STANDARD_CONSTRUCTOR()
    setMaxTableSize(3);
}

void CadrgMap::copyData(const CadrgMap& org, const bool)
{
    BaseClass::copyData(org);

    for (int i = 0; i < MAX_FILES; i++) {
        if (cadrgFiles[i] != nullptr) cadrgFiles[i]->unref();
        cadrgFiles[i] = org.cadrgFiles[i]->clone();
    }
    for (int i = 0; i < MAX_FILES; i++) {
        if (mergedCadrgFiles[i] != nullptr) mergedCadrgFiles[i]->unref();
        mergedCadrgFiles[i] = org.mergedCadrgFiles[i]->clone();
    }

    if (org.curCadrgFile!= nullptr) {
        if (curCadrgFile != nullptr) curCadrgFile->unref();
        curCadrgFile = org.curCadrgFile->clone();
    }

    if (org.stack != nullptr) {
        if (stack != nullptr) stack->unref();
        stack = org.stack;
        stack->ref();
    }
    if (org.mapLevel != nullptr) setMapLevel(org.mapLevel->c_str());

    maxTableSize = org.maxTableSize;
    numFiles = org.numFiles;
    initLevelLoaded = org.initLevelLoaded;
}

void CadrgMap::deleteData()
{
    for (int i = 0; i < MAX_FILES; i++) {
        if (cadrgFiles[i] != nullptr) cadrgFiles[i]->unref();
        cadrgFiles[i] = nullptr;
    }
    for (int i = 0; i < MAX_FILES; i++) {
        if (mergedCadrgFiles[i] != nullptr) mergedCadrgFiles[i]->unref();
        mergedCadrgFiles[i] = nullptr;
    }
    if (curCadrgFile != nullptr) curCadrgFile->unref();
    curCadrgFile = nullptr;

    if (stack != nullptr) stack->unref();
    stack = nullptr;
}

//------------------------------------------------------------------------------
// getNumberOfCadrgFiles() - Return total number of all files.
//------------------------------------------------------------------------------
int CadrgMap::getNumberOfCadrgFiles() {
    int num = 0;
    for (int i = 0; i < MAX_FILES; i++) if (mergedCadrgFiles[i] != nullptr) num++;
    return num;
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotPathnames() - Pathnames to the CADRG A.toc files.
//------------------------------------------------------------------------------
bool CadrgMap::setSlotPathnames(const base::PairStream* const x)
{
    bool ok = false;
    int count = 0;
    if (x != nullptr) {
        // Go through and set up our files based on the path names given
        base::IList::Item* item = (base::IList::Item*)x->getFirstItem();
        while (item != nullptr && count < MAX_FILES) {
            base::Pair* p = (base::Pair*)item->getValue();
            if (p != nullptr) {
                const auto text = dynamic_cast<base::String*>(p->object());
                if (text != nullptr) {
                    ok = setPathName(text->c_str());
                }
            }
            item = item->getNext();
            count++;
        }
    }

    sortMaps(count);
    return ok;
}

//------------------------------------------------------------------------------
// this goes through and sorts our maps, after we have counted how many we have
// in.
//------------------------------------------------------------------------------
void CadrgMap::sortMaps(const int count)
{
    // Now we have created all of our CadrgFile, we need to sort them by levels!
    if (count > 0) {
        std::cout << "CadrgMap - loading map files..." << std::endl;
        // Go through and see if we have matching scales
        // list of possible scales
        const auto scales = new base::String[MAX_FILES];
        int sCount = 0;
        // We are going to have to create a list of which scale to add and how many to add
        int num2Add[MAX_FILES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        for (int i = 0; i < MAX_FILES; i++) {
            // We are going to step through all of the files, and figure out which
            // file has the same levels.  As we do this, we will group all of the entries with
            // the same scale into one File, and then it will include all of the files with the same scale (level)
            if (cadrgFiles[i] != nullptr) {
                // First file, get the first entry, and figure out the scale
                int nb = cadrgFiles[i]->getNumBoundaries();
                for (int j = 0; j < nb; j++) {
                    CadrgTocEntry* toc  = cadrgFiles[i]->entry(j);
                    if (toc != nullptr) {
                        if (sCount == 0) {
                            // We are going to put the first scale into the list
                            scales[sCount].setStr(toc->getScale());
                            num2Add[sCount]++;
                            sCount++;
                        }
                        else {
                            // We have to compare our scales with the new TOC scale, to see if we need a new category
                            bool match = false;
                            for (int k = 0; k < sCount; k++) {
                                if (scales[k] != nullptr) {
                                    if (std::strcmp(scales[k].c_str(), toc->getScale()) == 0) {
                                        match = true;
                                        num2Add[k]++;
                                    }
                                }
                            }
                            // If we don't match it, we create a new scale
                            if (!match) {
                                num2Add[sCount]++;
                                scales[sCount++].setStr(toc->getScale());
                            }
                        }
                    }
                }
            }
        }
        int mFile = 0;
        // Clear our our merged files if we haven't done that
        for (int i = 0; i < MAX_FILES; i++) {
            if (mergedCadrgFiles[i] != nullptr) mergedCadrgFiles[i]->unref();
            mergedCadrgFiles[i] = nullptr;
        }
        // Now we know the number to add to the scount, so we can create our new files
        for (int y = 0; y < sCount; y++) {
            mergedCadrgFiles[mFile] = new CadrgFile();
            // Now go through our files, find the one that matches, and add it
            int tocIndex = 0;
            for (int i = 0; i < MAX_FILES; i++) {
                if (cadrgFiles[i] != nullptr) {
                    int nb = cadrgFiles[i]->getNumBoundaries();
                    for (int j = 0; j < nb; j++) {
                        CadrgTocEntry* toc = cadrgFiles[i]->entry(j);
                        if (toc != nullptr) {
                            if (std::strcmp(toc->getScale(), scales[y].c_str()) == 0) {
                                mergedCadrgFiles[mFile]->addTocEntry(toc, tocIndex++);
                            }
                        }
                    }
                }
            }
            mFile++;
        }
    }
    //// check our maps now
    //for (int i = 0; i < MAX_FILES; i++) {
    //    if (mergedCadrgFiles[i] != 0) {
    //        std::cout << "FILE # " << i << " : " << std::endl;
    //        int nb = mergedCadrgFiles[i]->getNumBoundaries();
    //        for (int j = 0; j < nb; j++) {
    //            maps::rpf::CadrgTocEntry* toc = mergedCadrgFiles[i]->entry(j);
    //            if (toc != 0) {
    //                std::cout << "Boundary # && Scale = " << j << ", " << toc->getScale() << std::endl;
    //            }
    //        }
    //    }
    //}

}

//------------------------------------------------------------------------------
// setSlotMaxTableSize() - Sets our max table size and array up.
//------------------------------------------------------------------------------
bool CadrgMap::setSlotMaxTableSize(const base::Integer* const x)
{
    bool ok{};
    if (x != nullptr) ok = setMaxTableSize(x->asInt());
    return ok;

}

//------------------------------------------------------------------------------
// setPathName() - Set our path name, which will also initialize our cadrg file.
//------------------------------------------------------------------------------
bool CadrgMap::setPathName(const char* aGenPathName)
{
    bool ok{};
    if (aGenPathName != nullptr) {
        if (cadrgFiles[numFiles] == nullptr) cadrgFiles[numFiles] = new CadrgFile();
        cadrgFiles[numFiles]->initialize(aGenPathName);
        numFiles++;
        ok = true;
    }
    return ok;
}

// ------------------------------------------------------------------------
// loadFrameToTexture() - Bring in a texture object, a color array, and the texture
// pager, and from that we will setup the texture parameters and load the
// texture object.
// ------------------------------------------------------------------------
void CadrgMap::loadFrameToTexture(graphics::Texture* tex, void* pixels)
{
    if (tex != nullptr) {
        tex->setPixels((GLubyte*)pixels);
        tex->setWrapS(GL_CLAMP);
        tex->setWrapT(GL_CLAMP);
        tex->setWidth(256);
        tex->setHeight(256);
        tex->setFormat(GL_RGB);
        tex->setMagFilter(GL_LINEAR);
        tex->setMinFilter(GL_LINEAR);
        tex->setNumComponents(3);
        tex->loadTexture();
    }
}

//------------------------------------------------------------------------------
// setMaxTableSize() - Sets up our Frame array.
//------------------------------------------------------------------------------
bool CadrgMap::setMaxTableSize(const int x)
{
    maxTableSize = x;
    int size = maxTableSize * 2;
    // Reset our list stack
    if (stack != nullptr) stack->unref();
    stack = new base::IList();
    for (int i = 0; i < size; i++) {
        const auto t = new CadrgFrame();
        stack->addHead(t);
        t->unref();
    }
    return true;
}

//------------------------------------------------------------------------------
// setZone - Set the current zone we are in.
//------------------------------------------------------------------------------
void CadrgMap::setZone(const int num, TexturePager* tp)
{
    if (curCadrgFile != nullptr) {
        int numBoundaries = curCadrgFile->getNumBoundaries();
        // Make sure we are within the zone boundaries, and make sure we aren't already using that TOC entry
        if (tp != nullptr) {
            // If we are a valid boundary, set our TOC, else clear us out.
            if (num != -1 && num < numBoundaries) tp->setToc(curCadrgFile->entry(num));
            else tp->setToc(nullptr);
        }
    }
}

// ------------------------------------------------------------------------
// zoomInMapLevel() - Zooms in to our next map level, if we have one.
// ------------------------------------------------------------------------
bool CadrgMap::zoomInMapLevel()
{
    bool ok = false;
    int index = 0;
    if (mapLevel != nullptr) {
        // Early out check, we have zoomed in as far as we can
        if (std::strcmp(mapLevel->c_str(), "5M") == 0) return false;

        const auto newLevel = new base::String();
        //Zoom in if we can
        if (std::strcmp(mapLevel->c_str(), "10M") == 0) {
            newLevel->setStr("5M");
            ok = true;
            index = 1;
        }
        else if (std::strcmp(mapLevel->c_str(), "1:250K") == 0) {
            newLevel->setStr("10M");
            ok = true;
            index = 2;
        }
        else if (std::strcmp(mapLevel->c_str(), "1:500K") == 0) {
            newLevel->setStr("1:250K");
            ok = true;
            index = 3;
        }
        else if (std::strcmp(mapLevel->c_str(), "1:1M") == 0) {
            newLevel->setStr("1:500K");
            ok = true;
            index = 4;
        }
        else if (std::strcmp(mapLevel->c_str(), "1:2M") == 0) {
            newLevel->setStr("1:1M");
            ok = true;
            index = 5;
        }
        else if (std::strcmp(mapLevel->c_str(), "1:5M") == 0) {
            newLevel->setStr("1:2M");
            ok = true;
            index = 6;
        }
        if (ok) {
            ok = setMapLevel(newLevel->c_str());
            if (!ok) {
                while (!ok && index > 0) {
                    index--;
                    if (index == 5) newLevel->setStr("1:1M");
                    if (index == 4) newLevel->setStr("1:500K");
                    if (index == 3) newLevel->setStr("1:250K");
                    if (index == 2) newLevel->setStr("10M");
                    if (index == 1) newLevel->setStr("5M");
                    ok = setMapLevel(newLevel->c_str());
                }
            }
        }
        newLevel->unref();
    }
    return ok;
}

// ------------------------------------------------------------------------
// zoomOutMapLevel() - Zoom out to the next map level, if we have any.
// ------------------------------------------------------------------------
bool CadrgMap::zoomOutMapLevel()
{
    bool ok = false;
    int index = 0;
    if (mapLevel != nullptr) {
        // Early out check, we have zoomed out as far as we can
        if (std::strcmp(mapLevel->c_str(), "1:5M") == 0) return false;

        const auto newLevel = new base::String();
        //Zoom in if we can
        if (std::strcmp(mapLevel->c_str(), "5M") == 0) {
            newLevel->setStr("10M");
            ok = true;
            index = 6;
        }
        else if (std::strcmp(mapLevel->c_str(), "10M") == 0) {
            newLevel->setStr("1:250K");
            ok = true;
            index = 5;
        }
        else if (std::strcmp(mapLevel->c_str(), "1:250K") == 0) {
            newLevel->setStr("1:500K");
            ok = true;
            index = 4;
        }
        else if (std::strcmp(mapLevel->c_str(), "1:500K") == 0) {
            newLevel->setStr("1:1M");
            ok = true;
            index = 3;
        }
        else if (std::strcmp(mapLevel->c_str(), "1:1M") == 0) {
            newLevel->setStr("1:2M");
            ok = true;
            index = 2;
        }
        else if (std::strcmp(mapLevel->c_str(), "1:2M") == 0) {
            newLevel->setStr("1:5M");
            ok = true;
            index = 1;
        }

        if (ok) {
            ok = setMapLevel(newLevel->c_str());
            if (!ok) {
                while (!ok && index > 0) {
                    index--;
                    if (index == 5) newLevel->setStr("1:250K");
                    if (index == 4) newLevel->setStr("1:500K");
                    if (index == 3) newLevel->setStr("1:1M");
                    if (index == 2) newLevel->setStr("1:2M");
                    if (index == 1) newLevel->setStr("1:5M");
                    ok = setMapLevel(newLevel->c_str());
                }
            }
        }
        // Now set our new level
        newLevel->unref();
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMapLevel() - Initially sets our map resolution level.
//------------------------------------------------------------------------------
bool CadrgMap::setSlotMapLevel(base::String* x)
{
    bool ok = false;
    if (mapLevel != nullptr) {
        mapLevel->unref();
        mapLevel = nullptr;
    }
    if (x != nullptr) {
        mapLevel = x;
        mapLevel->ref();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setMapLevel() - See if the given resolution level exists in our files, and if it does,
// set it as the current cadrg file.
//------------------------------------------------------------------------------
bool CadrgMap::setMapLevel(const char* x)
{
    bool found = false;
    int num = getNumberOfCadrgFiles();
    for (int i = 0; i < num; i++) {
        if (mergedCadrgFiles[i] != nullptr) {
            int nb = mergedCadrgFiles[i]->getNumBoundaries();
            for (int j = 0; j < nb; j++) {
                CadrgTocEntry* toc = mergedCadrgFiles[i]->entry(j);
                if (toc != nullptr) {
                    // If we find the right scale, and our current file isn't = to our last file, we set it.
                    if (std::strcmp(toc->getScale(), x) == 0) {
                        if (curCadrgFile != mergedCadrgFiles[i]) {
                            // This file has our current level on it
                            if (curCadrgFile != nullptr) curCadrgFile->unref();
                            curCadrgFile = mergedCadrgFiles[i];
                            curCadrgFile->ref();
                            // Now set our map level
                            if (mapLevel != nullptr) mapLevel->setStr(x);
                            else mapLevel = new base::String(x);
                        }
                        found = true;
                    }
                }
            }
        }
    }
    return found;
}

//------------------------------------------------------------------------------
// findBestZone() - Find the best zone based on the given lat/lon.
//------------------------------------------------------------------------------
int CadrgMap::findBestZone(const double lat, const double lon)
{
   int t = -1;
   int nb = 0;

   if (curCadrgFile != nullptr) {
      // Number of boundaries is actually the number of zones in this file!
      nb = curCadrgFile->getNumBoundaries();
      for (int i = 0; i < nb; i++) {
         CadrgTocEntry* toc = curCadrgFile->entry(i);
         if (toc != nullptr && toc->isMapImage()) {
            if (toc->isInZone(lat, lon)) return i;
         }
      }
   }
   return t;
}

//------------------------------------------------------------------------------
// getMapImage - Return our moving map image.
//------------------------------------------------------------------------------
MapDrawer* CadrgMap::getMapImage()
{
    MapDrawer* image = nullptr;
    base::Pair* pair = findByType(typeid(MapDrawer));
    if (pair != nullptr) image = dynamic_cast<MapDrawer*>(pair->object());
    return image;
}


//------------------------------------------------------------------------------
// getMapImage - Return our moving map image.
//------------------------------------------------------------------------------
const MapDrawer* CadrgMap::getMapImage() const
{
    const MapDrawer* image{};
    const base::Pair* pair{static_cast<const base::Pair*>(findByType(typeid(MapDrawer)))};
    if (pair != nullptr) image = dynamic_cast<const MapDrawer*>(pair->object());
    return image;
}

// ------------------------------------------------------------------------
// isValidFrame() - Is the row column specified within our current TOCS
// boundary rectangle of frames?
// ------------------------------------------------------------------------
bool CadrgMap::isValidFrame(const int row, const int column, TexturePager* tp)
{
   int vFrames = 0;
   int hFrames = 0;
   bool ok = false;
   if (tp != nullptr) {
      CadrgTocEntry* currentToc = tp->getToc();
      if (currentToc != nullptr) {
         vFrames = currentToc->getVertFrames();
         hFrames = currentToc->getHorizFrames();
      }
   }

   // The rows and columns we specified must fall in our frames and subframes
   if (row >= 0 && row < (vFrames * 6) && column >= 0 && column < (hFrames * 6)) ok =  true;

   return ok;
}

//------------------------------------------------------------------------------
// latLonToTileRowColumn() - Takes in a given lat/lon, and based on that,
// finds the closest tile in the map file to that lat/lon.  It also sets our
// origin row (in float pixels) so we know the exact location of the lat/lon,
// for calculating pixel offset later.  This also sets our initial origin row
// and origin column.
//------------------------------------------------------------------------------
void CadrgMap::latLonToTileRowColumn(const double lat, const double lon, float &originRow, float &originCol, int &tileRow, int &tileCol, float &pixelRow, float &pixelCol, TexturePager* tp)
{
    float row = 0, col = 0;

    latLonToPixelRowColumn(lat, lon, row, col, tp);

    // 256 pixels per tile, so we can figure out which tile we are actually on.
    int ppt = 256;

    // Determine our tile location(s) + pixel offset, and then set our original column, which is the aggregate of both
    // Tile row is the int result of the total row offset / pixels per tile (256)
    tileRow = static_cast<int>(row) / ppt;
    // The remainder is the pixel offset of that tile
    pixelRow = row - (tileRow * ppt);
    // Original row
    originRow = row;

    // Same here, only columns
    tileCol = static_cast<int>(col) / ppt;
    pixelCol = col - (tileCol*ppt);
    originCol = col;
}

// ------------------------------------------------------------------------
// latLonToPixelRowColumn() - This gets the aggregate pixel position of
// of specified lat/lon, and then sets the origin row and column.
// ------------------------------------------------------------------------
void CadrgMap::latLonToPixelRowColumn(const double lat, const double lon, float &originRow, float &originCol, TexturePager* tp)
{
    double nwLat = 0, nwLon = 0, vInt = 0, hInt = 0;
    if (tp != nullptr) {
        CadrgTocEntry* currentToc = tp->getToc();
        if (currentToc != nullptr) {
            nwLat = currentToc->getNWLat();
            nwLon = currentToc->getNWLon();
            // Interval is the spacing (nominal), in decimal degrees, between each pixel in the NS and EW direction.
            vInt = currentToc->getVertInterval();
            hInt = currentToc->getHorizInterval();
        }
    }

    double deltaLat = nwLat - lat;
    double deltaLon = lon - nwLon;

    if (vInt != 0) originRow = static_cast<float>(deltaLat / vInt);
    if (hInt != 0) originCol = static_cast<float>(deltaLon / hInt);
}

// ------------------------------------------------------------------------
// getTile() - Gets our pixels.
// ------------------------------------------------------------------------
void* CadrgMap::getPixels(const int row, const int column, TexturePager* tp)
{
    if (tp != nullptr) {
        CadrgTocEntry* currentToc = tp->getToc();
        if (currentToc != nullptr) {
            bool ok = isValidFrame(row, column, tp);
            if (!ok) {
                int vFrames = currentToc->getVertFrames();
                int hFrames = currentToc->getHorizFrames();
                std::cout << "Bad row,column " << row << "," << column << "   " << vFrames * 6 << "," << hFrames * 6 << std::endl;
                return nullptr;
            }

            int frameRow = row / 6;
            int frameCol = column / 6;
            CadrgFrameEntry* frameEntry = currentToc->getFrameEntry(frameRow, frameCol);
            if (frameEntry != nullptr) {
                frameEntry->loadClut();
                CadrgFrame* frame = frameEntry->getFrame();
                // If we don't have an entry, let's pull one from the stack
                if (frame == nullptr) {
                    base::IList::Item* item = stack->getFirstItem();
                    if (item != nullptr) {
                        CadrgFrame* x = (CadrgFrame*)(item->getValue());
                        if (x != nullptr) {
                            stack->removeHead();
                            // Tell it about the frame entry that owns it
                            x->load(frameEntry);
                            // Now on the other side, tell our frame entry about its child
                            frameEntry->setFrame(x);
                        }
                    }
                }
                // Get our frame again, because it now has been loaded
                frame = frameEntry->getFrame();
                if (frame != nullptr) {
                    // Setup our subframe for decompression
                    Subframe subframe;
                    // Decompress our subframe
                    frame->decompressSubframe(row, column, subframe);
                    // Set our color based on subframe image
                    for (int i = 0; i < 256; i++) {
                        for (int j = 0; j < 256; j++) {
                            CadrgClut::Rgb rgb = frameEntry->getClut().getColor(subframe.image[j][255-i]);
                            outTile.texel[i][j].red = rgb.red;
                            outTile.texel[i][j].green = rgb.green;
                            outTile.texel[i][j].blue = rgb.blue;
                        }
                    }
                }
            }
        }
    }

    return static_cast<void*>(&outTile);
}

// ------------------------------------------------------------------------
// releaseFrame() - Release the current frame within the frame entry
// at the specific row and column, if it exists.  This frees us space
// and is more efficient if the frame is not being used.
// ------------------------------------------------------------------------
void CadrgMap::releaseFrame(const int row, const int column, TexturePager* tp)
{
    if (tp != nullptr) {
        CadrgTocEntry* currentToc = tp->getToc();
        if (currentToc != nullptr) {
            bool ok = isValidFrame(row, column, tp);
            if (!ok) {
                std::cout << "Bad row,column " << row << "," << column << "   " << currentToc->getVertFrames() * 6 << "," << currentToc->getHorizFrames() * 6 << std::endl;
                return;
            }

            int frameRow = row / 6;
            int frameCol = column / 6;

            CadrgFrameEntry* frameEntry = currentToc->getFrameEntry(frameRow, frameCol);
            if (frameEntry != nullptr) {
            CadrgFrame* frame = frameEntry->getFrame();
                if (frame != nullptr) {
                    stack->addHead(frame);
                    frame->unref();
                    frame = nullptr;
                }
                frameEntry->setFrame(nullptr);
            }
        }
    }
}


// ------------------------------------------------------------------------
// getLevel() - Return our resolution level.
// ------------------------------------------------------------------------
const char* CadrgMap::getLevel()
{
    if (mapLevel != nullptr) {
       return mapLevel->c_str();
    }
    return nullptr;
}

//------------------------------------------------------------------------------
// updateData() - Update map data.
//------------------------------------------------------------------------------
void CadrgMap::updateData(double dt)
{
    BaseClass::updateData(dt);

    if (!initLevelLoaded && mapLevel != nullptr && !mapLevel->isEmpty()) {
        setMapLevel(mapLevel->c_str());
        initLevelLoaded = true;
    }
}

}
}

