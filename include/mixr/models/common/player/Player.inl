//------------------------------------------------------------------------------
// Class: Player
// Description: inline functions
//------------------------------------------------------------------------------
#ifndef __mixr_models_common_Player_Inline__
#define __mixr_models_common_Player_Inline__

// Player's Lat/lon (degs)
inline bool Player::getPositionLL(double* const lat, double* const lon) const
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
inline bool Player::getPositionLLA(double* const lat, double* const lon, double* const alt) const
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
inline const base::Vec3d& Player::getGeocPosition() const
{
   return posVecECEF;
}

// Altitude HAE (meters)
inline double Player::getAltitude() const
{
   return getAltitudeM();
}

// Altitude HAE (meters)
inline double Player::getAltitudeM() const
{
   return altitude;
}

// Altitude HAE (ft)
inline double Player::getAltitudeFt() const
{
   return getAltitude() * base::length::M2FT;
}

// Altitude AGL (meters)
inline double Player::getAltitudeAgl() const
{
   return altitude - static_cast<double>(tElev);
}

// Altitude AGL (meters)
inline double Player::getAltitudeAglM() const
{
   return getAltitudeAgl();
}

// Altitude AGL (ft)
inline double Player::getAltitudeAglFt() const
{
   return getAltitudeAgl() * base::length::M2FT;
}

// Player's total velocity (m/s)
inline double Player::getTotalVelocity() const
{
   return vp;
}

// Player's total velocity (ft/s)
inline double Player::getTotalVelocityFPS() const
{
   return getTotalVelocity() * base::length::M2FT;
}

// Player's total velocity (kts)
inline double Player::getTotalVelocityKts() const
{
   return getTotalVelocity() * base::length::M2NM * 3600.0f;
}

// Player's ground speed (m/s)
inline double Player::getGroundSpeed() const
{
   return gndSpd;
}

// Player's ground speed (ft/s)
inline double Player::getGroundSpeedFPS() const
{
   return getGroundSpeed() * base::length::M2FT;
}

// Player's ground speed (kts)
inline double Player::getGroundSpeedKts() const
{
   return getGroundSpeed() * base::length::M2NM * 3600.0f;
}

// Player's ground track (rad)
inline double Player::getGroundTrack() const
{
   return gndTrk;
}

// Player's ground track (rad)
inline double Player::getGroundTrackR() const
{
   return gndTrk;
}

// Player's ground track (degs)
inline double Player::getGroundTrackD() const
{
   return static_cast<double>(base::angle::R2DCC * gndTrk);
}

// Velocity vector; NED (m/s)
inline const base::Vec3d& Player::getVelocity() const
{
   return velVecNED;
}

// Acceleration vector; NED ((m/s)/s)
inline const base::Vec3d& Player::getAcceleration() const
{
   return accelVecNED;
}

// Body velocity vector: Nose/right/down (m/s)
inline const base::Vec3d& Player::getVelocityBody() const
{
   return velVecBody;
}

// Body Acceleration vector: Nose/right/down ((meters/second)/second)
inline const base::Vec3d& Player::getAccelerationBody() const
{
   return accelVecBody;
}

// Geocentric velocity vector [ x y z ] (m/s)
inline const base::Vec3d& Player::getGeocVelocity() const
{
   return velVecECEF;
}

// Geocentric acceleration vector [ x y z ] ((m/s)/s)
inline const base::Vec3d& Player::getGeocAcceleration() const
{
   return accelVecECEF;
}

// Initial geocentric position vector [ x y z ] (meters)
inline const base::Vec3d& Player::getInitGeocentricPosition() const
{
   return initGeoPosVec;
}

// Initial position vector is valid
inline bool Player::isInitGeocentricPositionValid() const
{
   return initGeoPosFlg;
}

// Player's initial latitude (degs)
inline double Player::getInitLatitude() const
{
   return initLat;
}

// Player's initial longitude (degs)
inline double Player::getInitLongitude() const
{
   return initLon;
}

// Initial lat/lon valid
inline bool Player::isInitLatLonValid() const
{
   return initLatLonFlg;
}

// Initial position vector; north/east from sim ref pt (m)
inline const base::Vec2d& Player::getInitPosition() const
{
   return initPosVec;
}

// Initial position vector is valid
inline bool Player::isInitPositionValid() const
{
   return initPosFlg;
}

// Initial altitude HAE (meters)
inline double Player::getInitAltitude() const
{
   return initAlt;
}

// Initial total velocity
inline double Player::getInitVelocity() const
{
   return initVp;
}

// Initial Euler angles (radians)
inline const base::Vec3d& Player::getInitAngles() const
{
   return initAngles;
}

// Returns the user defined camouflage type (or zero for none)
inline unsigned int Player::getCamouflageType() const
{
   return camouflage;
}

// Returns the player's damage state
inline double Player::getDamage() const
{
   return damage;
}

// Returns the player's smoke state
inline double Player::getSmoke() const
{
   return smoking;
}

// Returns the player's flames state
inline double Player::getFlames() const
{
   return flames;
}

// True if the player was just killed
inline bool Player::isJustKilled() const
{
   return justKilled;
}

// ID of the attacking player that killed us
inline int Player::killedByPlayerID() const
{
   return killedBy;
}

// True if the player is frozen (paused)
inline bool Player::isPositionFrozen() const
{
   return posFrz;
}

// True if the player's altitude is frozen
inline bool Player::isAltitudeFrozen() const
{
   return altFrz;
}

// True if the player's attitude is frozen
inline bool Player::isAttitudeFrozen() const
{
   return attFrz;
}

// True if the player's fuel is frozen
inline bool Player::isFuelFrozen() const
{
   return fuelFrz;
}

// True if the player is in crash override mode
inline bool Player::isCrashOverride() const
{
   return crashOverride;
}

// True if the player is in kill override mode
inline bool Player::isKillOverride() const
{
   return killOverride;
}

// True fi the player's kill removal flag is set
inline bool Player::isKillRemovalEnabled() const
{
   return killRemoval;
}

// True if altitude is slaved to an external model
inline bool Player::isAltitudeSlaved() const
{
   return altSlaved;
}

// True if position is slaved to an external model
inline bool Player::isPositionSlaved() const
{
   return posSlaved;
}

// Terrain elevation is valid (or at least was where it was set)
inline bool Player::isTerrainElevationValid() const
{
   return tElevValid;
}

// Terrain elevation at player (meters)
inline double Player::getTerrainElevation() const
{
   return tElev;
}

// Terrain elevation at player (meters)
inline double Player::getTerrainElevationM() const
{
   return getTerrainElevation();
}

// Returns terrain elevation (Feet)
inline double Player::getTerrainElevationFt() const
{
   return getTerrainElevation() * base::length::M2FT;
}

// Ground clamping offset from the terrain to our CG (meters)
inline double Player::getTerrainOffset() const
{
   return tOffset;
}

// interpolate our terrain from DTED if true
inline bool Player::isDtedTerrainInterpolationEnabled() const
{
   return interpTrrn;
}

// True if terrain elevation is required from the IG system; otherwise we use DTED, if available
inline bool Player::isTerrainElevationRequired() const
{
   return tElevReq;
}

// Returns the coordinate system currently being to update the player's position
inline Player::CoordSys Player::getCoordSystemInUse() const
{
   return useCoordSys;
}

// Get the current synchronized state
inline const SynchronizedState& Player::getSynchronizedState() const
{
   if (syncState1Ready)
      return syncState1;
   else
      return syncState2;
}

#endif
