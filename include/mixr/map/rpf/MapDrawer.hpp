
#ifndef __mixr_map_rpf_MapDrawer_HPP__
#define __mixr_map_rpf_MapDrawer_HPP__

#include "mixr/graphics/MapPage.hpp"
#include <array>

namespace mixr {
namespace base { class Boolean; class INumber; class String; }
namespace rpf {
class TexturePager;
class CadrgMap;

// -------------------------------------------------------------------------------
// Class: MapDrawer
//
// Description:
// This is the actual class that does the drawing of the map.  It cycles through
// its texture pagers and tells them where to reference, and how to draw.  Then
// texture pages draws the tiles.
//
// Subroutines:
//
// setSlotMapIntensity() -
//      bool MapDrawer::setSlotMapIntensity(const base::INumber* const x)
//
// setSlotDrawGridMode() -
//      bool MapDrawer::setSlotDrawGridMode(const base::INumber* const x)
//
// setSlotShowMap() -
//      bool MapDrawer::setSlotShowMap(const base::INumber* const x)
//
// setGridSize() - does our initial setup.
//      bool MapDrawer::setGridSize(const int aGridSize)
//
// setShowMap() - show our map or not?
//      bool MapDrawer::setShowMap(const bool x)
//
// setMap() - sets our parent map.
//      void MapDrawer::setMap(CadrgMap* map)
//
// updateZone() - update our zone in accordance with our texture pager.
//      void MapDrawer::updateZone(int curZone, int &oldZone, const int idx)
//
// drawFunc() - Draw the map.
//      void MapDrawer::drawFunc()
//
// determineScaling() - determine our reference scaling for each zone.
//      void MapDrawer::determineScaling(const int idx)
//
// drawMap() - called from draw fun, it tells our specific map to draw.
//      void MapDrawer::drawMap(const int zone, const int idx)
//
// drawTexture() - draws a tile at a specific position.
//      void MapDrawer::drawTexture(const int row, const int column, const int idx)
//
//
// -------------------------------------------------------------------------------
class MapDrawer : public graphics::MapPage
{
    DECLARE_SUBCLASS(MapDrawer, graphics::MapPage)

public:
    MapDrawer();

    // Set functions
    virtual bool setDrawGridMode(const bool dg)             { drawGrid = dg; return true; }
    virtual bool setMapIntensity(const double intensity)    { mapIntensity = intensity; return true; }
    virtual bool setShowMap(const bool);
    virtual void setMap(CadrgMap*);
    // this function initializes our size
    virtual bool setGridSize(const int);

    // Get functions
    double getMapIntensity()                                { return mapIntensity; }
    virtual void updateZone(int zone, int& selected, const int idx);

    void drawFunc() override;
    void updateData(const double dt = 0.0) override;

private:
    static const int MAX_PAGERS {2};

    enum { CENTER_PAGER, TOP_PAGER };   // Names of our pagers
    void drawTexture(const int row, const int column, const int idx);
    void goDrawGrid(const int row, const int column, const int idx);
    void drawMap(const int zone, const int idx);
    // Function to determine how to scale our non-center zones to line up with the center zone
    void determineScaling(const int idx);

    CadrgMap* myMap {};                 // The map
    double pixPerTile {256.0};          // Number of pixels per tile
    int gridSize {};                    // Size of the map grid 1x1 or 3x3 or NxN
    bool drawGrid {};                   // Will the grid be drawn?
    double mapIntensity {1.0};          // Map brightness factor

    double sinAng {};                   // SIN of heading
    double cosAng {};                   // COS of heading

    bool showMap {true};                // Flag used to command the drawing of actual textures or not

    double vpWL {};                     // Viewport width and height
    double vpHL {};                     // Viewport width and height

    std::array<TexturePager*, MAX_PAGERS> pagers {};   // List of texture pagers

    std::array<float, MAX_PAGERS> scalingNorth {}; // Scaling of our north size from center pager
    std::array<float, MAX_PAGERS> scalingEast {};  // Scaling of our east size from center pager
    std::array<int, MAX_PAGERS> zones {};          // Holds our zones
    std::array<int, MAX_PAGERS> textureRow {};     // Holds our row numbers of the textures to draw
    std::array<int, MAX_PAGERS> textureCol {};     // Holds our col numbers of the textures to draw
    std::array<float, MAX_PAGERS> pixelRow {};     // Pixel row position of the textures to draw
    std::array<float, MAX_PAGERS> pixelCol {};     // Pixel col position of the textures to draw
    std::array<float, MAX_PAGERS> originRow {};    // Pixel + texture row of the textures to draw
    std::array<float, MAX_PAGERS> originCol {};    // Pixel + texture col of the textures to draw

private:
   bool setSlotMapIntensity(const base::INumber* const);
   bool setSlotDrawGridMode(const base::Boolean* const);
   bool setSlotShowMap(const base::Boolean* const);
};

}
}

#endif

