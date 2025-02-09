
#include "mixr/terrain/ITerrain.hpp"

#include "mixr/base/colors/IColor.hpp"
#include "mixr/base/colors/Hsva.hpp"
#include "mixr/base/colors/Rgba.hpp"

#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/String.hpp"

#include "mixr/base/util/nav_utils.hpp"

#include "mixr/base/osg/Vec2d"
#include "mixr/base/osg/Vec3d"

#include <cmath>

namespace mixr {
namespace terrain {

IMPLEMENT_ABSTRACT_SUBCLASS(ITerrain, "ITerrain")

BEGIN_SLOTTABLE(ITerrain)
   "file",        // 1) Data file name
   "path",        // 2) Data path name
END_SLOTTABLE(ITerrain)

BEGIN_SLOT_MAP(ITerrain)
   ON_SLOT(1, setSlotFilename, base::String)
   ON_SLOT(2, setSlotPathname, base::String)
END_SLOT_MAP()

ITerrain::ITerrain()
{
   STANDARD_CONSTRUCTOR()
}

void ITerrain::copyData(const ITerrain& org, const bool)
{
   BaseClass::copyData(org);

   clearData();

   if (org.path != nullptr) {
      setPathname( static_cast<base::String*>(org.path->clone()) );
   } else {
      setPathname( nullptr );
   }

   if (org.file != nullptr) {
      setFilename( static_cast<base::String*>(org.file->clone()) );
   } else {
      setFilename( nullptr );
   }

   neLat = org.neLat;
   neLon = org.neLon;
   swLat = org.swLat;
   swLon = org.swLon;

   minElev = org.minElev;
   maxElev = org.maxElev;
}

void ITerrain::deleteData()
{
   clearData();

   setPathname(nullptr);
   setFilename(nullptr);
}

void ITerrain::reset()
{
   if ( !isDataLoaded() ) {
      loadData();
   }

   BaseClass::reset();
}

void ITerrain::clearData()
{
}

//------------------------------------------------------------------------------
// Access (get) functions
//------------------------------------------------------------------------------

// Returns the name of the datafile
const char* ITerrain::getFilename() const
{
   const char* p = nullptr;
   if (file != nullptr) {
      p = file->c_str();
   }
   return p;
}

// Returns the path to the datafiles
const char* ITerrain::getPathname() const
{
   const char* p = nullptr;
   if (path != nullptr) {
      // Our path
      p = path->c_str();
   } else {
      // See if we have a contain "Terrain" object that has a path set
      const ITerrain* q = static_cast<const ITerrain*>(findContainerByType(typeid(ITerrain)));
      if (q != nullptr) p = q->getPathname();
   }
   return p;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the name of the datafile
bool ITerrain::setFilename(const base::String* const msg)
{
   if (file != nullptr) file->unref();
   file = msg;
   if (file != nullptr) file->ref();
   return true;
}

// Sets the path to the datafiles
bool ITerrain::setPathname(const base::String* const msg)
{
   if (path != nullptr) path->unref();
   path = msg;
   if (path != nullptr) path->ref();
   return true;
}

// Minimum elevation in this database (meters)
bool ITerrain::setMinElevation(const double v)
{
   minElev = v;
   return true;
}

// Maximum elevation in this database (meters)
bool ITerrain::setMaxElevation(const double v)
{
   maxElev = v;
   return true;
}

// Southwest corner latitude of this database (degs)
bool ITerrain::setLatitudeSW(const double v)
{
   bool ok = false;
   if (v >= -90.0 && v <= 90.0) {
      swLat = v;
      ok = true;
   }
   return ok;
}

// Southwest corner longitude of this database (degs)
bool ITerrain::setLongitudeSW(const double v)
{
   bool ok = false;
   if (v >= -180.0 && v <= 180.0) {
      swLon = v;
      ok = true;
   }
   return ok;
}

// Northeast corner latitude of this database (degs)
bool ITerrain::setLatitudeNE(const double v)
{
   bool ok = false;
   if (v >= -90.0 && v <= 90.0) {
      neLat = v;
      ok = true;
   }
   return ok;
}

// Northeast corner longitude of this database (degs)
bool ITerrain::setLongitudeNE(const double v)
{
   bool ok = false;
   if (v >= -180.0 && v <= 180.0) {
      neLon = v;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Target occulting: returns true if a target point [ tgtLat tgtLon tgtAlt ] is
// occulted by the terrain as seen from the ref point [ refLat refLon refAlt ].
//------------------------------------------------------------------------------
bool ITerrain::targetOcculting(
      const double refLat,    // Ref latitude (degs)
      const double refLon,    // Ref longitude (degs)
      const double refAlt,    // Ref altitude (meters)
      const double tgtLat,    // Target latitude (degs)
      const double tgtLon,    // Target longitude (degs)
      const double tgtAlt     // Target altitude (meters)
   ) const
{
   // 1200 points gives us 100 meter data up to a distance
   // of one degree at the equator
   static const unsigned int MAX_POINTS = 1200;

   bool occulted = false;

   // Compute bearing and distance to target (flat earth)
   double brgDeg = 0.0;
   double distNM = 0.0;
   base::nav::fll2bd(refLat, refLon, tgtLat, tgtLon, &brgDeg, &distNM);
   double dist = (distNM * base::length::NM2M);

   // Number of points (default: 100M data)
   unsigned int numPts = static_cast<unsigned int>((dist / 100.0f) + 0.5f);
   if (numPts > MAX_POINTS) numPts = MAX_POINTS;

   // Get the elevations and check for target occulting
   if (numPts > 1) {

      // Arrays for the elevations
      double elevations[MAX_POINTS];

      // Valid flags
      bool validFlags[MAX_POINTS];
      for (unsigned int i = 0; i < numPts; i++) { validFlags[i] = false; }

      // Get the elevations
      unsigned int num = getElevations(elevations, validFlags, numPts, refLat, refLon,
                                       static_cast<double>(brgDeg), static_cast<double>(dist), false);

      // And check occulting
      if (num > 0) {
         occulted = occultCheck(elevations, validFlags, numPts, static_cast<double>(dist),
                                refAlt, tgtAlt);
      }
   }

   return occulted;
}

//------------------------------------------------------------------------------
// Target occulting #2: returns true if any terrain in the 'truBrg' direction
// for 'dist' meters occults (or masks) a target with a look angle of atan(tanLookAng)
//------------------------------------------------------------------------------
bool ITerrain::targetOcculting2(
      const double refLat,    // Ref latitude (degs)
      const double refLon,    // Ref longitude (degs)
      const double refAlt,    // Ref altitude (meters)
      const double truBrg,    // True direction angle from north to look (degs)
      const double dist,      // Distance to check (meters)
      const double tanLookAng // Tangent of the look angle
   ) const
{
   // 1200 points gives us 100 meter data up to a distance
   // of one degree at the equator
   static const unsigned int MAX_POINTS = 1200;

   bool occulted = false;

   // Number of points (default: 100M data)
   unsigned int numPts = static_cast<unsigned int>((dist / 100.0f) + 0.5f);
   if (numPts > MAX_POINTS) numPts = MAX_POINTS;

   // Get the elevations and check for target occulting
   if (numPts > 1) {

      // Arrays for the elevations
      double elevations[MAX_POINTS];

      // Valid flags
      bool validFlags[MAX_POINTS];
      for (unsigned int i = 0; i < numPts; i++) { validFlags[i] = false; }

      // Get the elevations
      unsigned int num = getElevations(elevations, validFlags, numPts, refLat, refLon,
                                       static_cast<double>(truBrg), static_cast<double>(dist), false);

      // And check occulting
      if (num > 0) {
         occulted = occultCheck2(elevations, validFlags, numPts, dist, refAlt, tanLookAng);
      }
   }

   return occulted;
}

//------------------------------------------------------------------------------
// Occulting check: returns true if a target at the altitude 'tgtAlt' and
// range 'range' is occulted by the elevation points as seen from the
// reference altitude 'refAlt'.
//------------------------------------------------------------------------------
bool ITerrain::occultCheck(
      const double* const elevations,  // The elevation array (meters)
      const bool* const validFlags,    // Valid elevation flag array (true if elevation was found)
      const unsigned int n,            // Size of the arrays
      const double range,              // Range (meters)
      const double refAlt,             // Ref altitude (meters)
      const double tgtAlt)             // Target altitude (meters)
{
   bool occulted = false;

   // Early out checks
   if (  elevations == nullptr ||    // The elevation array wasn't provided, or
         n < 2 ||              // there are too few points, or
         range <= 0            // the range is less than or equal to zero
         ) return occulted;

   // Tangent of the angle to the target point --
   // If the angle to any terrain point is greater than this
   // angle then the target is occulted by the terrain point
   const double tgtTan = (tgtAlt - refAlt) / range;

   // Loop through all elevation points looking for an angle
   // that's greater than our ref angle
   const double deltaRng = (range / (n - 1));
   double currentRange = 0;
   if (validFlags != nullptr) {
      // with valid flags
      for (unsigned int i = 1; i < (n-1) && !occulted; i++) {
         currentRange += deltaRng;
         if (validFlags[i]) {
            const double tstTan = (elevations[i] - refAlt) / currentRange;
            if (tstTan >= tgtTan) {
               occulted = true;
            }
         }
      }
   }
   else {
      // without valid flags
      for (unsigned int i = 1; i < (n-1) && !occulted; i++) {
         currentRange += deltaRng;
         const double tstTan = (elevations[i] - refAlt) / currentRange;
         if (tstTan >= tgtTan) {
            occulted = true;
         }
      }
   }

   return occulted;
}

//------------------------------------------------------------------------------
// Occulting check #2: returns true if any of the tangents of the angles (from
// level) to each elevation point, as seen from the ref altitude, refAlt, is
// greater than the tangent of the 'look' angle, 'tanLookAng'.
//------------------------------------------------------------------------------
bool ITerrain::occultCheck2(
      const double* const elevations, // The elevation array (meters)
      const bool* const validFlags, // Valid elevation flag array (true if elevation was found)
      const unsigned int n,         // Size of the arrays
      const double range,           // Range (meters)
      const double refAlt,          // Ref altitude (meters)
      const double tanLookAng)      // Tangent of the look angle
{
   bool occulted = false;

   // Early out checks
   if (  elevations == nullptr ||   // The elevation array wasn't provided, or
         n < 2 ||                   // there are too few points, or
         range <= 0                 // the range is less than or equal to zero
         ) return occulted;

   // Loop through all elevation points looking for an angle
   // that's greater than our ref angle
   const double deltaRng = (range / (n - 1));
   double currentRange = 0;
   if (validFlags != nullptr) {
      // with valid flags
      for (unsigned int i = 1; i < (n-1) && !occulted; i++) {
         currentRange += deltaRng;
         if (validFlags[i]) {
            const double tstTan = (elevations[i] - refAlt) / currentRange;
            if (tstTan >= tanLookAng) {
               occulted = true;
            }
         }
      }
   } else {
      // without valid flags
      for (unsigned int i = 1; i < (n-1) && !occulted; i++) {
         currentRange += deltaRng;
         const double tstTan = (elevations[i] - refAlt) / currentRange;
         if (tstTan >= tanLookAng) {
            occulted = true;
         }
      }
   }

   return occulted;
}


//------------------------------------------------------------------------------
// Vertical Beam Width and Shadow Check --
// Sets an array of mask flags; the flags are set true if the point
// is masked (in shadow or out of beam) as seen from the reference
// altitude over the first point.  Returns true if successful
//------------------------------------------------------------------------------
bool ITerrain::vbwShadowChecker(
      bool* const maskFlags,          // The array of mask flags
      const double* const elevations, // The elevation array (meters)
      const bool* const validFlags,   // (Optional) Valid elevation flag array (true if elevation was found)
      const unsigned int n,           // Size of the arrays
      const double range,             // Range (meters)
      const double refAlt,            // Ref altitude (meters)
      const double beamAngle,         // Center beam elevation angle (degs)
      const double beamWidth)         // Total beam width angle (degs)
{
   // Early out checks
   if (  maskFlags == nullptr ||      // The mask flag array wasn't provided, or
         elevations == nullptr ||     // the elevation array wasn't provided, or
         n < 2 ||                     // there are too few points, or
         range <= 0                   // the range is less than or equal to zero
         ) return false;

   // Compute the upper and lower edges of the beam
   double beamUpper = beamAngle + beamWidth/2.0f;
   if (beamUpper >  89.9999f) beamUpper =  89.9999f;
   double beamLower = beamAngle - beamWidth/2.0f;
   if (beamLower < -89.9999f) beamLower = -89.9999f;

   // tangents of the upper and lower edges of the beam
   double tanUpper = std::tan(beamUpper * static_cast<double>(base::angle::D2RCC));
   double tanLower = std::tan(beamLower * static_cast<double>(base::angle::D2RCC));

   // Loop through all other elevation points -- keep track of the current max
   // tangent value and flag as terrain masked all points with tangent
   // values less than the current.

   const double deltaRng = (range / (n - 1));
   double currentRange = 0;
   if (validFlags != nullptr) {
      // with valid flags
      double tanLookAngle = 0.0;
      for (unsigned int i = 0; i < n; i++) {
         if (validFlags[i]) {
            // Valid data -- compute the tangent of the look angle to the point and
            // see if it's within the upper and lower tangent boundaries
            if (currentRange > 0) {
               tanLookAngle = (elevations[i] - refAlt) / currentRange;
            } else {
               tanLookAngle = (elevations[i] - refAlt) / 1.0f;
            }
            if (tanLookAngle >= tanLower && tanLookAngle <= tanUpper) {
               // In beam and not in shadow
               maskFlags[i] = false;
               tanLower = tanLookAngle;
            } else {
               // Out of beam or in shadow
               maskFlags[i] = true;
            }
         } else {
            // Set the mask flag for all invalid data
            maskFlags[i] = true;
         }
         currentRange += deltaRng;
      }
   } else {
      // without valid flags
      double tanLookAngle = 0;
      for (unsigned int i = 0; i < n; i++) {
         // Compute the tangent of the look angle to the point and
         // see if it's within the upper and lower tangent boundaries
         if (currentRange > 0) {
            tanLookAngle = (elevations[i] - refAlt) / currentRange;
         } else {
            tanLookAngle = (elevations[i] - refAlt) / 1.0f;
         }
         if (tanLookAngle >= tanLower && tanLookAngle <= tanUpper) {
            // In beam and not in shadow
            maskFlags[i] = false;
            tanLower = tanLookAngle;
         } else {
            // Out of beam or in shadow
            maskFlags[i] = true;
         }
         currentRange += deltaRng;
      }
   }

   return true;
}

//------------------------------------------------------------------------------
// aac() -- Compute Aspect Angle Cosines; computes the cosine of the angle
// inwhich the beam hits the terrain.
//------------------------------------------------------------------------------
bool ITerrain::aac(
      double* const aacData,        // The array for the aspect angle cosines
      const double* const elevData, // The elevation array (meters)
      const bool* const maskFlags,  // (Optional) The array of mask flags
      const unsigned int n,         // Size of the arrays
      const double range,           // Range (meters)
      const double refAlt           // Ref altitude (meters)
   )
{
   // Early out checks
   if (  aacData == nullptr ||      // The AAC data array wasn't provided, or
         elevData == nullptr ||     // the elevation array wasn't provided, or
         n < 3 ||                   // there are too few points, or
         range <= 0                 // the range is less than or equal to zero
         ) return false;


   // m1 will be used as a normalized vector along a line from the
   // ref point to the current terrain point.
   base::Vec2d m1;

   // m2 will be used is a normalized vector along a line perpendicular
   // to the current terrain slope.
   base::Vec2d m2;

   // ---
   // For all elevation points, except the end points, compute the dot product
   // of a vector in the direction of the beam with a vector perpendicular to
   // the terrain.
   // ---

   const double deltaRng = (range / (n - 1));
   const double deltaRng2 = deltaRng * 2.0f;
   double currentRange = deltaRng;

   if (maskFlags != nullptr) {
      // with mask flags
      for (unsigned int i = 1; i < (n-1); i++) {
         if (!maskFlags[i]) {
            m1.set(currentRange, elevData[i] - refAlt);
            m1.normalize();
            m2.set((elevData[i+1] - elevData[i-1]),-deltaRng2);
            m2.normalize();
            double v = m1 * m2;
            if (v < 0.0) v = 0;
            aacData[i] = v;
         } else {
            aacData[i] = 0.0f;
         }
         currentRange += deltaRng;
      }
   }

   else {
      // without mask flags
      for (unsigned int i = 1; i < (n-1); i++) {
         m1.set(currentRange, elevData[i] - refAlt);
         m1.normalize();
         m2.set((elevData[i+1] - elevData[i-1]),-deltaRng2);
         m2.normalize();
         double v = m1 * m2;
         if (v < 0.0) v = 0;
         aacData[i] = v;
         currentRange += deltaRng;
      }
   }

   // ---
   // Set the endpoints to the next to endpoints values;
   // ---
   aacData[0]   = aacData[1];
   aacData[n-1] = aacData[n-2];

   return true;
}

//------------------------------------------------------------------------------
// cLight() -- Computes the columnated lighting effect for each point
//------------------------------------------------------------------------------
bool ITerrain::cLight(
      double* const ldata,          // The array for the lighting factors
      const double* const elevData, // The elevation array (meters)
      const bool* const maskFlags,  // (Optional) The array of mask flags
      const unsigned int n,         // Size of the arrays
      const double range,           // Range (meters)
      const base::Vec2d& lv         // Columnated lighting vector
   )
{
   // Early out checks
   if (  ldata == nullptr ||        // The lighting data array wasn't provided, or
         elevData == nullptr ||     // the elevation array wasn't provided, or
         n < 3 ||                   // there are too few points, or
         range <= 0                 // the range is less than or equal to zero
         ) return false;


   // m2 will be used is a normalized vector along a line perpendicular
   // to the current terrain slope.
   base::Vec2d m2;

   // ---
   // For all elevation points, except the end points, compute the dot product
   // of a vector in the direction of the beam with a vector perpendicular to
   // the terrain.
   // ---

   const double deltaRng = (range / (n - 1));
   const double deltaRng2 = deltaRng * 2.0f;

   if (maskFlags != nullptr) {
      // with mask flags
      for (unsigned int i = 1; i < (n-1); i++) {
         if (!maskFlags[i]) {
            m2.set((elevData[i+1] - elevData[i-1]),-deltaRng2);
            m2.normalize();
            double v = lv * m2;
            if (v < 0.0) v = 0;
            ldata[i] = v;
         } else {
            ldata[i] = 0.0f;
         }
      }
   }

   else {
      // without mask flags
      for (unsigned int i = 1; i < (n-1); i++) {
         m2.set((elevData[i+1] - elevData[i-1]),-deltaRng2);
         m2.normalize();
         double v = lv * m2;
         if (v < 0.0) v = 0;
         ldata[i] = v;
      }
   }

   // ---
   // Set the endpoints to the next to endpoints values;
   // ---
   ldata[0]   = ldata[1];
   ldata[n-1] = ldata[n-2];

   return true;
}

//------------------------------------------------------------------------------
// Converts an elevation to a color (or gray scale)
//------------------------------------------------------------------------------
bool ITerrain::getElevationColor(
      const double elevation,          // Elevation
      const double minz,               // Min elevation
      const double maxz,               // Max elevation
      const base::Hsva** colorTable,   // Color table
      const unsigned int numColors,    // Number of colors
      base::Vec3d& rgb)                // Color
{

   // early out test
   if ( colorTable == nullptr || // no color table, or
        numColors < 1      // no colors, or
         ) return false;

   // HSV color
   base::Hsva colorHsv;

   if ( elevation < minz ) {
      // too low is the first color
      colorHsv = *colorTable[0];
   } else if ( elevation >= maxz ) {
      // too high is the last color
      colorHsv = *colorTable[numColors-1];
   } else if (numColors == 2) {
      // Only two, that's easy
      colorHsv.colorInterpolate(elevation, minz, maxz, *colorTable[0], *colorTable[1]);
   } else {

      // Full range: min to max
      double elevRange = maxz - minz;

      // elevation steps between colors
      double elevSteps = elevRange / (numColors-1);

      // delta elevation between min to ref elevations
      double deltaElev = elevation - minz;

      // Lower color table index
      unsigned int idx = static_cast<unsigned int>(deltaElev/elevSteps);
      if (idx > (numColors - 2)) {
         idx = numColors - 2;
      }

      // Lower & upper elevation limits
      const double lowLimit = idx * elevSteps;
      const double highLimit = (idx+1) * elevSteps;

      // Interpolate between colors
      colorHsv.colorInterpolate(deltaElev, lowLimit, highLimit, *colorTable[idx], *colorTable[(idx+1)]);
   }

   // Return the RGB value of this new color
   const base::Vec3d* p = colorHsv.getRGB();
   if (p != nullptr) rgb = *p;

   return true;
}

}
}
