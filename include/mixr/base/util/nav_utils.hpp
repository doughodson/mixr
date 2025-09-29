
#ifndef __mixr_base_util_nav_utils_HPP__
#define __mixr_base_util_nav_utils_HPP__

#include "mixr/base/EarthModel.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"
#include "mixr/base/osg/Matrixd"

#include "mixr/base/qty/angles.hpp"
#include "mixr/base/qty/lengths.hpp"
#include "mixr/base/util/math_utils.hpp"

namespace mixr {
namespace base {
namespace nav {

// Vec2d and Vec3d indexes
enum { INORTH, IEAST, IDOWN };   // North, east and down (NED) position vector
enum { IX, IY, IZ };             // [ X Y Z ] vector
enum { ILAT, ILON, IALT };       // Lat/lon/alt position vector
enum { IPHI, ITHETA, IPSI };     // Euler angles
enum { IROLL, IPITCH, IYAW };    // Euler angles

//------------------------------------------------------------------------------
// Navigation constants
//------------------------------------------------------------------------------
const double WGS84_A  = 6378137.0;                  // WGS-84 semi major axis (Meters)
const double WGS84_B  = 6356752.314245;             // WGS-84 semi minor axis (Meters)
const double WGS84_F  = (WGS84_A-WGS84_B)/WGS84_A;  // WGS-84 flattening
const double WGS84_E2 = WGS84_F*(2.0 - WGS84_F);    // WGS-84 Eccentricity Squared

const double ERADM    = WGS84_A;                    // Equatorial radius (WGS84 major axis) (Meters)
const double ERAD     = ERADM/1852.0;               // Equatorial radius (WGS84 major axis) (NM)
const double ERAD60   = 3437.74677078;              // Earth Radius (NM) (1 Arc Deg = 60 NM)

//------------------------------------------------------------------------------
// Local WGS84 constants used by the legacy SubrDis conversion routines
//------------------------------------------------------------------------------
const double ellipseA    = 6378137;                         // semi-major axis in meters
const double ellipseF    = 1/298.257223563;                 // flattening
const double ellipseE2   = ellipseF * (2.0 - ellipseF);     // Eccentricity Squared
const double ellipseB    = ellipseA * (1.0 - ellipseF);     // semi-minor axis in meters
const double ellipseAsq  = ellipseA * ellipseA;
const double ellipseBsq  = ellipseB * ellipseB;

//==============================================================================
// Navigation Functions
//==============================================================================

   //------------------------------------------------------------------------------
   // Flat-earth Brg/Dist to Lat/Lon, with Earth model (fdb2llE)
   //
   // Computes the destination (target) lat/lon from starting (ref)
   // point given the distance and initial bearing using flat earth projection
   // with an optional earth model, em
   //------------------------------------------------------------------------------
   bool fbd2llE(
         const double slat,                 // IN:  Starting (reference) latitude (degs)
         const double slon,                 // IN:  Starting (reference) longitude (degs)
         const double brg,                  // IN:  True bearing (degs)
         const double dist,                 // IN:  Distance (ground range) (nm)
         double* const dlat,                // OUT: Destination latitude (degs)
         double* const dlon,                // OUT: Destination longitude (degs)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   //------------------------------------------------------------------------------
   // Flat-earth Lat/Lon to Brg/Dist, with Earth model (fll2bdE)
   //
   // Computes the initial bearing and the distance from the starting
   // lat/lon (ref point) to the destination lat/lon using flat earth projection
   // with an optional earth model, em
   //------------------------------------------------------------------------------
   bool fll2bdE(
         const double slat,                 // IN:  Starting (reference) latitude (degs)
         const double slon,                 // IN:  Starting (reference) longitude (degs)
         const double dlat,                 // IN:  Destination latitude (degs)
         const double dlon,                 // IN:  Destination longitude (degs)
         double* const brg,                 // OUT: True bearing (degs)
         double* const dist,                // OUT: Distance (ground range) (nm)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   //------------------------------------------------------------------------------
   // Flat-earth Brg/Dist to Lat/Lon, with Spherical earth (fbd2llS)
   //
   // Computes the destination (target) lat/lon from starting (ref)
   // point given the distance and initial bearing using flat earth projection
   // with a spherical earth radius of 'ERAD60'
   //------------------------------------------------------------------------------
   bool fbd2llS(
         const double slat,           // IN:  Starting (reference) latitude (degs)
         const double slon,           // IN:  Starting (reference) longitude (degs)
         const double brg,            // IN:  True bearing (degs)
         const double dist,           // IN:  Distance (ground range) (nm)
         double* const dlat,          // OUT: Destination latitude (degs)
         double* const dlon           // OUT: Destination longitude (degs)
      );

   //------------------------------------------------------------------------------
   // Flat-earth Lat/Lon to Brg/Dist, with Spherical earth (fll2bdS)
   //
   // Computes the initial bearing and the distance from the starting
   // lat/lon (ref point) to the destination lat/lon using flat earth projection
   // with a spherical earth radius of 'ERAD60'
   //------------------------------------------------------------------------------
   bool fll2bdS(
         const double slat,           // IN:  Starting (reference) latitude (degs)
         const double slon,           // IN:  Starting (reference) longitude (degs)
         const double dlat,           // IN:  Destination latitude (degs)
         const double dlon,           // IN:  Destination longitude (degs)
         double* const brg,           // OUT: True bearing (degs)
         double* const dist           // OUT: Distance (ground range) (nm)
      );

   //------------------------------------------------------------------------------
   // Legacy flat-earth projection; same as fbd2llS()
   //------------------------------------------------------------------------------
   bool fbd2ll(
         const double slat,           // IN:  Starting (reference) latitude (degs)
         const double slon,           // IN:  Starting (reference) longitude (degs)
         const double brg,            // IN:  True bearing (degs)
         const double dist,           // IN:  Distance (ground range) (nm)
         double* const dlat,          // OUT: Destination latitude (degs)
         double* const dlon           // OUT: Destination longitude (degs)
      );

   //------------------------------------------------------------------------------
   // Legacy flat-earth projection; same as fll2bdS()
   //------------------------------------------------------------------------------
   bool fll2bd(
         const double slat,           // IN:  Starting (reference) latitude (degs)
         const double slon,           // IN:  Starting (reference) longitude (degs)
         const double dlat,           // IN:  Destination latitude (degs)
         const double dlon,           // IN:  Destination longitude (degs)
         double* const brg,           // OUT: True bearing (degs)
         double* const dist           // OUT: Distance (ground range) (nm)
      );


   //------------------------------------------------------------------------------
   // Converts a single set of Azimuth, Elevation and Range values to X, Y, Z (aer2xyz)
   //------------------------------------------------------------------------------

   // Using body angles
   bool aer2xyz(
         Vec3d* const pos,            // OUT: Position vector array (NED, player centered)  [meters]
         const Matrixd& rm,           // IN:  NED to body rotational matrix (see computeRotationalMatrix())
         const double az,             // IN:  Azimuth (body)  (radians)
         const double el,             // IN:  Elevation (body, positive up, radians)
         const double rng             // IN:  Range [meters]
      );

   // Using NED angles
   bool aer2xyz(
         Vec3d* const pos,            // OUT: Position vector array (NED, player centered)  [meters]
         const double az,             // IN:  Azimuth (NED, radians)
         const double el,             // IN:  Elevation (NED, positive up, radians)
         const double rng             // IN:  Range [meters]
      );

   //------------------------------------------------------------------------------
   // Convert 'n' sets of Azimuth, Elevation and Range values to X, Y, Z (aer2xyzArray)
   //------------------------------------------------------------------------------

   // Using body angles
   bool aer2xyzArray(
         Vec3d* const pos,            // OUT: position vector array (NED, player centered)  [meters]
         const Matrixd& rm,           // IN:  NED to body rotational matrix (see computeRotationalMatrix())
         const double* const az,      // IN:  azimuth (body) array  (radians)
         const double* const el,      // IN:  elevation (body) array (positive up) (radians)
         const double* const rng,     // IN:  range array [meters]
         const unsigned int n         // IN:  number of sets to convert
      );

   // Using NED angles
   bool aer2xyzArray(
         Vec3d* const pos,            // OUT: position vector array (NED, player centered)  [meters]
         const double* const az,      // IN:  azimuth (NED) array  (radians)
         const double* const el,      // IN:  elevation (NED) array (positive up) (radians)
         const double* const rng,     // IN:  range array [meters]
         const unsigned int n         // IN:  number of sets to convert
      );

   //------------------------------------------------------------------------------
   // Converts a single set of X, Y, Z values to Azimuth, Elevation and Range (xyz2aer)
   //------------------------------------------------------------------------------

   // Computing NED angles
   bool xyz2aer(
         Vec3d* const aer,            // OUT: position vector  (aer)   [deg,deg,meters]
         const double x,              // IN:  x                        [meters]
         const double y,              // IN:  y                        [meters]
         const double z               // IN:  z                        [meters]
      );

   // Computing body angles
   bool xyz2aer(
         Vec3d* const aer,            // OUT: position vector  (aer)   [deg,deg,meters]
         const Matrixd& rm,           // IN:  NED to body rotational matrix (see computeRotationalMatrix())
         const double x,              // IN:  x                        [meters]
         const double y,              // IN:  y                        [meters]
         const double z               // IN:  z                        [meters]
      );

   //------------------------------------------------------------------------------
   // Great circle Brg/Dist to Lat/Lon (gbd2ll)
   //
   // Computes the destination lat/lon from starting (ref)
   // point given distance and initial bearing.
   //------------------------------------------------------------------------------

   // ---
   // Great circle method #1 -- Elliptical earth model
   // ---
   bool gbd2ll(
         const double slat,                 // IN:  Starting (reference) latitude (degs)
         const double slon,                 // IN:  Starting (reference) longitude (degs)
         const double brg,                  // IN:  True bearing (degs)
         const double dist,                 // IN:  Distance (ground range) (nm)
         double* const dlat,                // OUT: Destination latitude (degs)
         double* const dlon,                // OUT: Destination longitude (degs)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   // ---
   // Great circle method #2 -- Spherical earth with radius 'ERAD60'
   // reference: "ARRL"
   // ---
   bool gbd2llS(
         const double slat,           // IN:  Starting (reference) latitude (degs)
         const double slon,           // IN:  Starting (reference) longitude (degs)
         const double brg,            // IN:  True bearing (degs)
         const double dist,           // IN:  Distance (ground range) (nm)
         double* const dlat,          // OUT: Destination latitude (degs)
         double* const dlon           // OUT: Destination longitude (degs)
      );

   //------------------------------------------------------------------------------
   // Great circle Lat/Lon to Brg/Dist (gll2bd)
   //
   // Computes the initial bearing and the distance from the starting
   // lat/lon (ref point) to the destination lat/lon using great circle methods.
   //------------------------------------------------------------------------------

   // ---
   // Great circle method #1 -- Elliptical earth model
   // ---
   bool gll2bd(
         const double slat,                 // IN:  Starting (reference) latitude (degs)
         const double slon,                 // IN:  Starting (reference) longitude (degs)
         const double dlat,                 // IN:  Destination latitude (degs)
         const double dlon,                 // IN:  Destination longitude (degs)
         double* const brg,                 // OUT: True bearing (degs)
         double* const dist,                // OUT: Distance (ground range) (nm)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   // ---
   // Great circle method #2 -- Spherical earth with radius 'ERAD60'
   // reference: "ARRL"
   // ---
   bool gll2bdS(
         const double slat,           // IN:  Starting (reference) latitude (degs)
         const double slon,           // IN:  Starting (reference) longitude (degs)
         const double dlat,           // IN:  Destination latitude (degs)
         const double dlon,           // IN:  Destination longitude (degs)
         double* const brg,           // OUT: True bearing (degs)
         double* const dist           // OUT: Distance (ground range) (nm)
      );

   //------------------------------------------------------------------------------
   // Great circle Lat/Lon/Alt to Brg/Dist (glla2bd)
   //
   // Computes the initial bearing, slant range, ground distance and
   // elevation angle from the starting lat/lon (ref point) to the destination
   // lat/lon using great circle methods.
   // (elevation does not account for earth curvature at this time)
   //------------------------------------------------------------------------------
   bool glla2bd(
         const double slat,                 // IN:  Starting (reference) latitude (degs)
         const double slon,                 // IN:  Starting (reference) longitude (degs)
         const double salt,                 // IN:  Starting (reference) altitude (meters)
         const double dlat,                 // IN:  Destination latitude (degs)
         const double dlon,                 // IN:  Destination longitude (degs)
         const double dalt,                 // IN:  Destination altitude (meters)
         double* const brg,                 // OUT: True bearing (degs)
         double* const slantRng,            // OUT: Slant range (nm)
         double* const dist,                // OUT: Distance (ground range) (nm)
         double* const elev=nullptr,        // OUT: Optional: elevation angle (degs)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   // ---
   // Method #2 -- Spherical earth with radius 'ERAD60'
   // ---
   bool glla2bdS(
         const double slat,                 // IN:  Starting (reference) latitude (degs)
         const double slon,                 // IN:  Starting (reference) longitude (degs)
         const double salt,                 // IN:  Starting (reference) altitude (meters)
         const double dlat,                 // IN:  Destination latitude (degs)
         const double dlon,                 // IN:  Destination longitude (degs)
         const double dalt,                 // IN:  Destination altitude (meters)
         double* const brg,                 // OUT: True bearing (degs)
         double* const slantRng,            // OUT: Slant range (nm)
         double* const dist,                // OUT: Distance (ground range) (nm)
         double* const elev = nullptr       // OUT: (optional) elevation angle (degs)
      );


   //------------------------------------------------------------------------------
   // Vincenty direct method Lat/Lon to Brg/Dist (vbd2ll)
   //
   // Computes the destination lat/lon from starting (ref) point
   // given the distance and initial bearing using the Vincenty
   // direct method.
   //
   //    reference: "Direct and Inverse Solutions of Geodesics on the Ellipsoid with
   //                Applications of Nested Equations"  by Thaddeus Vincenty
   //------------------------------------------------------------------------------
   bool vbd2ll(
         const double slat,                 // IN:  Starting (Reference) latitude (degs)
         const double slon,                 // IN:  Starting (Reference) longitude (degs)
         const double brg,                  // IN:  True bearing (degs)
         const double dist,                 // IN:  Distance (ground range) (nm)
         double* const dlat,                // OUT: Target latitude (degs)
         double* const dlon,                // OUT: Target longitude (degs)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   //------------------------------------------------------------------------------
   // Vincenty inverse method Lat/Lon to Brg/Dist (vll2bd)
   //
   // Computes the initial and final bearings and the distance from the starting
   // lat/lon (ref point) to destination lat/lon using the Vincenty Inverse Method.
   //
   //    reference: "Direct and Inverse Solutions of Geodesics on the Ellipsoid with
   //               Applications of Nested Equations"  by Thaddeus Vincenty
   //
   //------------------------------------------------------------------------------
   bool vll2bd(
         const double slat,                 // IN:  Starting (reference) latitude (degs)
         const double slon,                 // IN:  Starting (reference) longitude (degs)
         const double dlat,                 // IN:  Destination latitude (degs)
         const double dlon,                 // IN:  Destination longitude (degs)
         double* const brg,                 // OUT: True initial bearing (degs)
         double* const dist,                // OUT: Geodesic distance (nm)
         double* const brg2=nullptr,        // OUT: Optional: true final bearing (degs)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

//==============================================================================
// Matrix generators
//==============================================================================

   //------------------------------------------------------------------------------
   // Compute World transformation Matrix (computeWorldMatrix) from lat/lon (degrees)
   //
   //       World (ECEF) <==> Inertial (NED)
   //
   //    World (ECEF): origin is at the center of the earth where X+ points to zero
   //    latitude and zero longitude (i.e., equator at the Greenwich meridian) and Z+
   //    points to the north pole -- ECEF rotates with the earth.
   //
   //    Inertial (NED): the local tangent plane where X+ points north, Y+ points east,
   //    and Z+ points 'down' and is perpendicular to the tangent plane
   //
   // output:
   //    M = Ry[-(90+lat)] * Rz[lon]
   //
   // Usage:
   //    Vi  = M * Vw;
   //    Vw = Vi * M;    // same as Vw = M[T] * Vi
   //       Vi is an initial (NED) vector
   //       Vw is a world (ECEF) vector
   //------------------------------------------------------------------------------
   bool computeWorldMatrix(
         const double latD,           // IN:  Reference latitude (degs)
         const double lonD,           // IN:  Reference longitude (degs)
         Matrixd* const m             // OUT: Matrix
      );

   //------------------------------------------------------------------------------
   // Compute Rotational transformation Matrix (computeRotationalMatrix),
   // with optional sin/cos values) from Euler angles
   //
   //    Reference system <==> Rotating system
   //
   // output:
   //    M  = Rx[phi] * Ry[theta] * Rz[psi]
   //
   // Usage:
   //    Vb = M * Vi;
   //    Vi = Vb * M;   // same as Vi = M(t) * Vb
   //       Vb is a body (or rotating) vector
   //       Vi is an initial (or fixed) vector
   //------------------------------------------------------------------------------

   // Version using radians
   bool computeRotationalMatrix(
         const double phi,                // IN:  Roll angle (radians)
         const double theta,              // IN:  Pitch angle (radians)
         const double psi,                // IN:  Yaw angle (radians)
         Matrixd* const rm,               // OUT: Rotational matrix
         Vec2d* const scPhi=nullptr,     // OUT: Sin/Cos of phi (Optional)
         Vec2d* const scTht=nullptr,     // OUT: Sin/Cos of theta (Optional)
         Vec2d* const scPsi=nullptr      // OUT: Sin/Cos of psi (Optional)
      );

   // Version using degrees
   bool computeRotationalMatrixDeg(
         const double phiD,               // IN:  Roll angle (degrees)
         const double thetaD,             // IN:  Pitch angle (degrees)
         const double psiD,               // IN:  Yaw angle (degrees)
         Matrixd* const rm,               // OUT: Rotational matrix
         Vec2d* const scPhi=nullptr,      // OUT: Sin/Cos of phi (Optional)
         Vec2d* const scTht=nullptr,      // OUT: Sin/Cos of theta (Optional)
         Vec2d* const scPsi=nullptr       // OUT: Sin/Cos of psi (Optional)
      );

   // Vec3 version
   bool computeRotationalMatrix(
         const Vec3d& angles,             // IN:  Euler angles [ phi theta psi ] (radians)
         Matrixd* const m,                // OUT: Matrix
         Vec2d* const scPhi=nullptr,      // OUT: Sin/Cos of phi (Optional)
         Vec2d* const scTht=nullptr,      // OUT: Sin/Cos of theta (Optional)
         Vec2d* const scPsi=nullptr       // OUT: Sin/Cos of psi (Optional)
      );

   // Vec3 version using degrees
   bool computeRotationalMatrixDeg(
         const Vec3d& angles,             // IN:  Euler angles [ phi theta psi ] (degrees)
         Matrixd* const m,                // OUT: Matrix
         Vec2d* const scPhi=nullptr,      // OUT: Sin/Cos of phi (Optional)
         Vec2d* const scTht=nullptr,      // OUT: Sin/Cos of theta (Optional)
         Vec2d* const scPsi=nullptr       // OUT: Sin/Cos of psi (Optional)
      );


   //------------------------------------------------------------------------------
   // Compute Euler Angles (computeEulerAngles), and optionally the sin/cos for
   // each, from a rotational matrix as defined by computeRotationalMatrix() above.
   //
   //    Input matrix = Rx[phi] * Ry[theta] * Rz[psi]
   //------------------------------------------------------------------------------

   // Version using radians
   bool computeEulerAngles(
         const Matrixd& rm,               // IN:  Rotational matrix
         Vec3d* const angles,             // OUT: Euler angles (radians)
         Vec2d* const scPhi=nullptr,      // OUT: Sin/Cos of phi (Optional)
         Vec2d* const scTht=nullptr,      // OUT: Sin/Cos of theta (Optional)
         Vec2d* const scPsi=nullptr       // OUT: Sin/Cos of psi (Optional)
      );

   // Version using degrees
   bool computeEulerAnglesDeg(
         const Matrixd& rm,               // IN:  Rotational matrix
         Vec3d* const anglesD,            // OUT: Euler angles (degrees)
         Vec2d* const scPhi=nullptr,      // OUT: Sin/Cos of phi (Optional)
         Vec2d* const scTht=nullptr,      // OUT: Sin/Cos of theta (Optional)
         Vec2d* const scPsi=nullptr       // OUT: Sin/Cos of psi (Optional)
      );


//==============================================================================
// Latitude, longitude and altitude (LLA) to/from NED vector conversion functions
//==============================================================================

   //----------------------------------------------------------
   // NED position vector to LLA using a flat earth projection
   // with an optional earth model (default: WGS-84)
   //----------------------------------------------------------
   bool convertPosVec2llE(
         const double slat,                 // IN:  Reference latitude (degs)
         const double slon,                 // IN:  Reference longitude (degs)
         const double sinSlat,              // IN:  Sine of ref latitude
         const double cosSlat,              // IN:  Cosine of ref latitude
         const Vec3d& pos,                  // IN:  NED position vector from ref point (Meters)
         double* const lat,                 // OUT: Latitude (degs)
         double* const lon,                 // OUT: Longitude (degs)
         double* const alt,                 // OUT: Altitude (meters)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   bool convertPosVec2llE(
         const double slat,                 // IN:  Reference latitude (degs)
         const double slon,                 // IN:  Reference longitude (degs)
         const Vec3d& pos,                  // IN:  NED position vector from ref point (Meters)
         double* const lat,                 // OUT: Latitude (degs)
         double* const lon,                 // OUT: Longitude (degs)
         double* const alt,                 // OUT: Altitude (meters)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   //----------------------------------------------------------
   // NED position vector to LLA using a flat earth projection
   // with a spherical earth radius of ERAD60
   //----------------------------------------------------------
   bool convertPosVec2llS(
         const double slat,         // IN:  Reference latitude (degs)
         const double slon,         // IN:  Reference longitude (degs)
         const double cosSlat,      // IN:  Cosine of ref latitude
         const Vec3d& pos,          // IN:  NED position vector from ref point (Meters)
         double* const lat,         // OUT: Latitude (degs)
         double* const lon,         // OUT: Longitude (degs)
         double* const alt          // OUT: Altitude (meters)
      );

   //----------------------------------------------------------
   // Legacy functions; use convertPosVec2llS()
   //----------------------------------------------------------
   // using double Vec3
   bool convertPosVec2LL(
         const double slat,         // IN:  Reference latitude (degs)
         const double slon,         // IN:  Reference longitude (degs)
         const Vec3d& pos,          // IN:  NED position vector from ref point (Meters)
         double* const lat,         // OUT: Latitude (degs)
         double* const lon,         // OUT: Longitude (degs)
         double* const alt          // OUT: Altitude (meters)
      );
   // using float Vec3
   bool convertPosVec2LL(
         const double slat,         // IN:  Reference latitude (degs)
         const double slon,         // IN:  Reference longitude (degs)
         const Vec3f& pos,          // IN:  NED position vector from ref point (Meters)
         double* const lat,         // OUT: Latitude (degs)
         double* const lon,         // OUT: Longitude (degs)
         double* const alt          // OUT: Altitude (meters)
      );

   //----------------------------------------------------------
   // LLA to NED position vector using a flat earth projection
   // with an optional earth model (default: WGS-84)
   //----------------------------------------------------------
   bool convertLL2PosVecE(
         const double slat,                 // IN:  Reference latitude (degs)
         const double slon,                 // IN:  Reference longitude (degs)
         const double sinSlat,              // IN:  Sine of ref latitude
         const double cosSlat,              // IN:  Cosine of ref latitude
         const double lat,                  // IN:  Latitude (degs)
         const double lon,                  // IN:  Longitude (degs)
         const double alt,                  // IN:  Altitude (meters)
         Vec3d* const pos,                  // OUT: NED position vector from ref point (Meters)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   bool convertLL2PosVecE(
         const double slat,                 // IN:  Reference latitude (degs)
         const double slon,                 // IN:  Reference longitude (degs)
         const double lat,                  // IN:  Latitude (degs)
         const double lon,                  // IN:  Longitude (degs)
         const double alt,                  // IN:  Altitude (meters)
         Vec3d* const pos,                  // OUT: NED position vector from ref point (Meters)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   //----------------------------------------------------------
   // LLA to NED position vector using a flat earth projection
   // with a spherical earth radius of ERAD60
   //----------------------------------------------------------
   bool convertLL2PosVecS(
         const double slat,         // IN:  Reference latitude (degs)
         const double slon,         // IN:  Reference longitude (degs)
         const double cosSlat,      // IN:  Cosine of ref latitude
         const double lat,          // IN:  Latitude (degs)
         const double lon,          // IN:  Longitude (degs)
         const double alt,          // IN:  Altitude (meters)
         Vec3d* const pos           // OUT: NED position vector from ref point (Meters)
      );

   //----------------------------------------------------------
   // Legacy functions; use convertLL2PosVecS()
   //----------------------------------------------------------
   // using double Vec3
   bool convertLL2PosVec(
         const double slat,         // IN:  Reference latitude (degs)
         const double slon,         // IN:  Reference longitude (degs)
         const double lat,          // IN:  Latitude (degs)
         const double lon,          // IN:  Longitude (degs)
         const double alt,          // IN:  Altitude (meters)
         Vec3d* const pos           // OUT: NED position vector from ref point (Meters)
      );
   // using float Vec3
   bool convertLL2PosVec(
         const double slat,         // IN:  Reference latitude (degs)
         const double slon,         // IN:  Reference longitude (degs)
         const double lat,          // IN:  Latitude (degs)
         const double lon,          // IN:  Longitude (degs)
         const double alt,          // IN:  Altitude (meters)
         Vec3f* const pos           // OUT: NED position vector from ref point (Meters)
      );


//==============================================================================
// Coordinate conversion functions
//    Earth centered, earth fixed (ECEF) coordinates <==> Geodetic coordinates
//
// Currently hard coded to WGS84 geodetic coordinates
//==============================================================================

   //----------------------------------------------------------
   // Convert ECEF (XYZ coordinates) to Geodetic (LLA coordinates)
   //----------------------------------------------------------
   bool convertEcef2Geod(
         const double x,                    // IN:  ECEF X component   (meters)
         const double y,                    // IN:  ECEF Y component   (meters)
         const double z,                    // IN:  ECEF Z component   (meters)
         double* const pLat,                // OUT: Geodetic latitude  (degrees)
         double* const pLon,                // OUT: Geodetic longitude (degrees)
         double* const pAlt,                // OUT: Geodetic altitude  (meters)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   // Using Vec3d vectors
   bool convertEcef2Geod(
         const Vec3d& vec,                  // IN:  ECEF [ IX IY IZ ]
         Vec3d* const lla,                  // OUT: Geodetic [ ILAT ILON IALT ]
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   // Using array vectors
   bool convertEcef2Geod(
         const double vec[3],               // IN:  ECEF [ IX IY IZ ]
         double lla[3],                     // OUT: Geodetic [ ILAT ILON IALT ]
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   //----------------------------------------------------------
   // Convert Geodetic (LLA coordinates) to ECEF (XYZ coordinates)
   //----------------------------------------------------------
   bool convertGeod2Ecef(
         const double lat,                  // IN:  Geodetic latitude  (degrees)
         const double lon,                  // IN:  Geodetic longitude (degrees)
         const double alt,                  // IN:  Geodetic altitude  (meters)
         double* const pX,                  // OUT: ECEF X component   (meters)
         double* const pY,                  // OUT: ECEF Y component   (meters)
         double* const pZ,                  // OUT: ECEF Z component   (meters)
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   // Using Vec3d vectors
   bool convertGeod2Ecef(
         const Vec3d& lla,                  // IN:  Geodetic [ ILAT ILON IALT ]
         Vec3d* const ecef,                 // OUT: ECEF [ IX IY IZ ]
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );

   // Using array vectors
   bool convertGeod2Ecef(
         const double lla[3],               // IN:  Geodetic [ ILAT ILON IALT ]
         double ecef[3],                    // OUT: ECEF [ IX IY IZ ]
         const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
      );


//==============================================================================
// Euler angle conversion functions
//    Body/NED <==> Body/ECEF
//==============================================================================

   //----------------------------------------------------------
   // Convert geodetic Euler angles (body/NED) to geocentric Euler angles (body/ECEF)
   //----------------------------------------------------------

   bool convertGeodAngles2EcefAngles(
         const Matrixd& wm,      // IN:  World matrix
         const Matrixd& rm,      // IN:  Geodetic rotational matrix (body/NED directional cosines)
         Vec3d* const vc         // OUT: Geocentric (ECEF) angles  [ phi theta psi ] (radians)
      );

   bool convertGeodAngles2EcefAngles(
         const Matrixd& wm,      // IN:  World matrix
         const Vec3d& rpy,       // IN:  Geodetic angles [ roll  pitch yaw ] (radians)
         Vec3d* const vc         // OUT: Geocentric (ECEF) angles  [ phi theta psi ] (radians)
      );

   bool convertGeodAngles2EcefAngles(
         const Vec2d& ll,        // IN:  Geodetic position  [ ILAT ILON ] [ degs degs ]
         const Vec3d& rpy,       // IN:  Geodetic angles [ roll  pitch yaw ] (radians)
         Vec3d* const vc         // OUT: Geocentric (ECEF) angles  [ phi theta psi ] (radians)
      );

   //----------------------------------------------------------
   // Convert geocentric Euler angles (body/ECEF) to geodetic Euler angles (body/NED)
   //----------------------------------------------------------
   bool convertEcefAngles2GeodAngles(
         const Matrixd& wm,      // IN:  World matrix
         const Matrixd& rm,      // IN:  Geocentric rotational matrix (ECEF/body directional cosines)
         Vec3d* const vd         // Out: Geodetic angles (radians) [ roll pitch yaw ]
      );

   bool convertEcefAngles2GeodAngles(
         const Matrixd& wm,      // IN:  World matrix
         const Vec3d& rpy,       // IN:  Geocentric angles (radians) [ phi theta psi ]
         Vec3d* const vd         // Out: Geodetic angles (radians) [ roll pitch yaw ]
      );

   bool convertEcefAngles2GeodAngles(
         const Vec2d& ll,        // IN:  Geodetic position [ ILAT ILON ] [ degs degs ]
         const Vec3d& rpy,       // IN:  Geocentric angles (radians) [ phi theta psi ]
         Vec3d* const vd         // Out: Geodetic angles (radians) [ roll pitch yaw ]
      );


//==============================================================================
// Geodetic (WGS-84) to/from Geocentric (ECEF) Conversion functions
//   (Legacy from SubrDis -- no longer being used --)
//==============================================================================

   // Converts geodetic (WGS84) to geocentric (ECEF) coordinates (From SubrDis)
   bool getGeocCoords(
      const double geodPos[3],   // IN:  Geodetic (WGS84)  [ ILAT ILON IALT ] [ degs degs meters ]
      double geocPos[3]          // OUT: Geocentric (ECEF) [ IX IY IZ ] (meters)
   );

   // Converts geocentric to geodetic coordinates (From SubrDis)
   bool getGeodCoords(
      const double geocPos[3],   // IN:  Geocentric (ECEF) [ IX IY IZ ] (meters)
      double geodPos[3]          // OUT: Geodetic (WGS84) [ ILAT ILON IALT ] [ degs degs meters ]
   );

   // Converts geocentric angles to geodetic angles (From SubrDis)
   bool getGeodAngle(
      const double geodPos[2],      // IN:  Geodetic position [ ILAT ILON ] [ degs degs ]
      const double geocAngle[3],    // IN:  Geocentric Euler angles (radians) [ IPHI ITHETA IPSI ]
      double geodAngle[3]           // OUT: Geodetic Euler angles (radians) [ IPHI ITHETA IPSI ]
   );

   // Converts geodetic angles to geocentric angles (From SubrDis)
   bool getGeocAngle(
      const double geodPos[2],      // IN:  Geodetic position [ ILAT ILON ] [ degs degs ]
      const double geodAngle[3],    // IN:  Geodetic Euler angles (radians) [ IPHI ITHETA IPSI ]
      double geocAngle[3]           // OUT: Geocentric Euler angles (radians) [ IPHI ITHETA IPSI ]
   );

   // Converts geocentric to geodetic (From SubrDis)
   bool getSimPosAccVel(
      const double geocPos[3],   // IN:  Geocentric position vector [ IX IY IZ ] (meters)
      const double geocVel[3],   // IN:  Geocentric velocity vector [ IX IY IZ ] (m/sec)
      const double geocAcc[3],   // IN:  Geocentric acceleration vector [ IX IY IZ ] (m/sec/sec)
      double geodPos[3],         // OUT: Geodetic (WGS84) position [ ILAT ILON IALT ] [ degs degs meters ]
      double geodVel[3],         // OUT: Geodetic velocity vector [ IX IY IZ ] (m/sec)
      double geodAcc[3]          // OUT: Geodetic acceleration vector [ IX IY IZ ] (m/sec/sec)
   );

   // Converts geodetic to geocentric (From SubrDis)
   bool getWorldPosAccVel(
      const double geodPos[3],   // IN:  Geodetic (WGS84) position [ ILAT ILON IALT ] [ degs degs meters ]
      const double geodVel[3],   // IN:  Geodetic velocity vector [ IX IY IZ ] (m/sec)
      const double geodAcc[3],   // IN:  Geodetic acceleration vector [ IX IY IZ ] (m/sec/sec)
      double geocPos[3],         // OUT: Geocentric position vector [ IX IY IZ ] (meters)
      double geocVel[3],         // OUT: Geocentric velocity vector [ IX IY IZ ] (m/sec)
      double geocAcc[3]          // OUT: Geocentric acceleration vector [ IX IY IZ ] (m/sec/sec)
   );


//==============================================================================
// Universal Transverse Mercator (UTM)
//==============================================================================

   // Converts Latitude, Longitude to UTM Northing and Easting
   bool convertLL2Utm(
      const double lat,                  // IN:  Latitude  [DEG]
      const double lon,                  // IN:  Longitude [DEG]
      char* const  pLatZone,             // OUT: Latitude Zone
      int*  const  pLonZone,             // OUT: Longitude Zone
      double* const pNorthing,           // OUT: Northing [M]
      double* const pEasting,            // OUT: Easting  [M]
      const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
   );

   // Converts UTM Northing and Easting to Latitude, Longitude
   bool convertUtm2LL(
      const double northing,             // IN:  Northing [M]
      const double easting,              // IN:  Easting  [M]
      char latZone,                      // IN:  Latitude Zone
      int  lonZone,                      // IN:  Longitude Zone
      double* const pLat,                // OUT: Latitude  [DEG]
      double* const pLon,                // OUT: Longitude [DEG]
      const EarthModel* const em=nullptr // IN:  Pointer to an optional earth model (default: WGS-84)
   );

#include "mixr/base/util/nav_utils.inl"

}
}
}


#endif



