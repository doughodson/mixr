//------------------------------------------------------------------------------
// Class: IPlayer
// Description: inline functions
//------------------------------------------------------------------------------
#ifndef __mixr_models_IPlayer_Inline__
#define __mixr_models_IPlayer_Inline__

// Player's Lat/lon (degs)
inline bool IPlayer::getPositionLL(double* const lat, double* const lon) const
{
   bool ok{};
   if (lat != nullptr && lon != nullptr) {
      *lat = latitude;
      *lon = longitude;
      ok = true;
   }
   return ok;
}

// Players Lat/Lon (degs) and altitude (meters)
inline bool IPlayer::getPositionLLA(double* const lat, double* const lon, double* const alt) const
{
   bool ok{};
   if (lat != nullptr && lon != nullptr) {
      *lat = latitude;
      *lon = longitude;
      *alt = getAltitudeM();
      ok = true;
   }
   return ok;
}

// Geocentric position vector [ x y z ] (meters)
inline const base::Vec3d& IPlayer::getGeocPosition() const
{
   return posVecECEF;
}

// Altitude HAE (meters)
inline double IPlayer::getAltitude() const
{
   return getAltitudeM();
}

// Altitude HAE (meters)
inline double IPlayer::getAltitudeM() const
{
   return altitude;
}

// Altitude HAE (ft)
inline double IPlayer::getAltitudeFt() const
{
   return getAltitude() * base::length::M2FT;
}

// Altitude AGL (meters)
inline double IPlayer::getAltitudeAgl() const
{
   return altitude - static_cast<double>(tElev);
}

// Altitude AGL (meters)
inline double IPlayer::getAltitudeAglM() const
{
   return getAltitudeAgl();
}

// Altitude AGL (ft)
inline double IPlayer::getAltitudeAglFt() const
{
   return getAltitudeAgl() * base::length::M2FT;
}

// Player's total velocity (m/s)
inline double IPlayer::getTotalVelocity() const
{
   return vp;
}

// Player's total velocity (ft/s)
inline double IPlayer::getTotalVelocityFPS() const
{
   return getTotalVelocity() * base::length::M2FT;
}

// Player's total velocity (kts)
inline double IPlayer::getTotalVelocityKts() const
{
   return getTotalVelocity() * base::length::M2NM * 3600.0f;
}

// Player's ground speed (m/s)
inline double IPlayer::getGroundSpeed() const
{
   return gndSpd;
}

// Player's ground speed (ft/s)
inline double IPlayer::getGroundSpeedFPS() const
{
   return getGroundSpeed() * base::length::M2FT;
}

// Player's ground speed (kts)
inline double IPlayer::getGroundSpeedKts() const
{
   return getGroundSpeed() * base::length::M2NM * 3600.0f;
}

// Player's ground track (rad)
inline double IPlayer::getGroundTrack() const
{
   return gndTrk;
}

// Player's ground track (rad)
inline double IPlayer::getGroundTrackR() const
{
   return gndTrk;
}

// Player's ground track (degs)
inline double IPlayer::getGroundTrackD() const
{
   return static_cast<double>(base::angle::R2DCC * gndTrk);
}

// Velocity vector; NED (m/s)
inline const base::Vec3d& IPlayer::getVelocity() const
{
   return velVecNED;
}

// Acceleration vector; NED ((m/s)/s)
inline const base::Vec3d& IPlayer::getAcceleration() const
{
   return accelVecNED;
}

// Body velocity vector: Nose/right/down (m/s)
inline const base::Vec3d& IPlayer::getVelocityBody() const
{
   return velVecBody;
}

// Body Acceleration vector: Nose/right/down ((meters/second)/second)
inline const base::Vec3d& IPlayer::getAccelerationBody() const
{
   return accelVecBody;
}

// Geocentric velocity vector [ x y z ] (m/s)
inline const base::Vec3d& IPlayer::getGeocVelocity() const
{
   return velVecECEF;
}

// Geocentric acceleration vector [ x y z ] ((m/s)/s)
inline const base::Vec3d& IPlayer::getGeocAcceleration() const
{
   return accelVecECEF;
}

// Initial geocentric position vector [ x y z ] (meters)
inline const base::Vec3d& IPlayer::getInitGeocentricPosition() const
{
   return initGeoPosVec;
}

// Initial position vector is valid
inline bool IPlayer::isInitGeocentricPositionValid() const
{
   return initGeoPosFlg;
}

// Player's initial latitude (degs)
inline double IPlayer::getInitLatitude() const
{
   return initLat;
}

// Player's initial longitude (degs)
inline double IPlayer::getInitLongitude() const
{
   return initLon;
}

// Initial lat/lon valid
inline bool IPlayer::isInitLatLonValid() const
{
   return initLatLonFlg;
}

// Initial position vector; north/east from sim ref pt (m)
inline const base::Vec2d& IPlayer::getInitPosition() const
{
   return initPosVec;
}

// Initial position vector is valid
inline bool IPlayer::isInitPositionValid() const
{
   return initPosFlg;
}

// Initial altitude HAE (meters)
inline double IPlayer::getInitAltitude() const
{
   return initAlt;
}

// Initial total velocity
inline double IPlayer::getInitVelocity() const
{
   return initVp;
}

// Initial Euler angles (radians)
inline const base::Vec3d& IPlayer::getInitAngles() const
{
   return initAngles;
}

// Returns the user defined camouflage type (or zero for none)
inline unsigned int IPlayer::getCamouflageType() const
{
   return camouflage;
}

// Returns the player's damage state
inline double IPlayer::getDamage() const
{
   return damage;
}

// Returns the player's smoke state
inline double IPlayer::getSmoke() const
{
   return smoking;
}

// Returns the player's flames state
inline double IPlayer::getFlames() const
{
   return flames;
}

// True if the player was just killed
inline bool IPlayer::isJustKilled() const
{
   return justKilled;
}

// ID of the attacking player that killed us
inline int IPlayer::killedByPlayerID() const
{
   return killedBy;
}

// True if the player is frozen (paused)
inline bool IPlayer::isPositionFrozen() const
{
   return posFrz;
}

// True if the player's altitude is frozen
inline bool IPlayer::isAltitudeFrozen() const
{
   return altFrz;
}

// True if the player's attitude is frozen
inline bool IPlayer::isAttitudeFrozen() const
{
   return attFrz;
}

// True if the player's fuel is frozen
inline bool IPlayer::isFuelFrozen() const
{
   return fuelFrz;
}

// True if the player is in crash override mode
inline bool IPlayer::isCrashOverride() const
{
   return crashOverride;
}

// True if the player is in kill override mode
inline bool IPlayer::isKillOverride() const
{
   return killOverride;
}

// True fi the player's kill removal flag is set
inline bool IPlayer::isKillRemovalEnabled() const
{
   return killRemoval;
}

// True if altitude is slaved to an external model
inline bool IPlayer::isAltitudeSlaved() const
{
   return altSlaved;
}

// True if position is slaved to an external model
inline bool IPlayer::isPositionSlaved() const
{
   return posSlaved;
}

// Terrain elevation is valid (or at least was where it was set)
inline bool IPlayer::isTerrainElevationValid() const
{
   return tElevValid;
}

// Terrain elevation at player (meters)
inline double IPlayer::getTerrainElevation() const
{
   return tElev;
}

// Terrain elevation at player (meters)
inline double IPlayer::getTerrainElevationM() const
{
   return getTerrainElevation();
}

// Returns terrain elevation (Feet)
inline double IPlayer::getTerrainElevationFt() const
{
   return getTerrainElevation() * base::length::M2FT;
}

// Ground clamping offset from the terrain to our CG (meters)
inline double IPlayer::getTerrainOffset() const
{
   return tOffset;
}

// interpolate our terrain from DTED if true
inline bool IPlayer::isDtedTerrainInterpolationEnabled() const
{
   return interpTrrn;
}

// True if terrain elevation is required from the IG system; otherwise we use DTED, if available
inline bool IPlayer::isTerrainElevationRequired() const
{
   return tElevReq;
}

// Returns the coordinate system currently being to update the player's position
inline IPlayer::CoordSys IPlayer::getCoordSystemInUse() const
{
   return useCoordSys;
}

// Get the current synchronized state
inline const SynchronizedState& IPlayer::getSynchronizedState() const
{
   if (syncState1Ready)
      return syncState1;
   else
      return syncState2;
}

#endif
