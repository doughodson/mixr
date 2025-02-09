
#ifndef __mixr_terrain_QuadMap_HPP__
#define __mixr_terrain_QuadMap_HPP__

#include "mixr/terrain/ITerrain.hpp"
#include <array>

namespace mixr {
namespace terrain {
class DataFile;

//------------------------------------------------------------------------------
// Class: QuadMap
// Description: Manage up to 4 elevation files in a 2x2 pattern
// Factory name: QuadMap
//------------------------------------------------------------------------------
class QuadMap : public ITerrain
{
   DECLARE_SUBCLASS(QuadMap, ITerrain)

public:
   QuadMap();

   unsigned int getNumDataFiles() const;
   const ITerrain* getDataFile(const unsigned int i) const;
   bool setDataFile(const unsigned int i, ITerrain* newDF);

   // ---
   // simulation::Terrain interface
   // ---

   bool isDataLoaded() const override;

   // Locates an array of (at least two) elevation points (and sets valid flags if found)
   // returns the number of points found within this QuadMap
   unsigned int getElevations(
         double* const elevations,     // The elevation array (meters)
         bool* const validFlags,       // Valid elevation flag array (true if elevation was found)
         const unsigned int n,         // Size of elevation and valdFlags arrays
         const double lat,             // Starting latitude (degs)
         const double lon,             // Starting longitude (degs)
         const double direction,       // True direction (heading) angle of the data (degs)
         const double maxRng,          // Range to last elevation point (meters)
         const bool   interp = false   // Interpolate between elevation posts (default: false)
      ) const override;

   // Locates an elevation value (meters) for a given reference point and returns
   // it in 'elev'.  Function returns true if successful, otherwise 'elev' is unchanged.
   bool getElevation(
         double* const elev,           // The elevation value (meters)
         const double lat,             // Reference latitude (degs)
         const double lon,             // Reference longitude (degs)
         const bool interp = false     // Interpolate between elevation posts (default: false)
      ) const override;

   void reset() override;

protected:
   virtual void findDataFiles();           // Initializes the channel array

   void clearData() override;

private:
   static const unsigned int MAX_DATA_FILES{4};            // Only 4 files (as in Quad!)

   std::array<const ITerrain*, MAX_DATA_FILES> dataFiles{}; // Terrain data files
   unsigned int numDataFiles{};                            // Number of data files

   bool loadData() override;
};

}
}

#endif
