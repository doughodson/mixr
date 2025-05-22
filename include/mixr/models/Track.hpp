
#ifndef __mixr_models_common_Track_HPP__
#define __mixr_models_common_Track_HPP__

#include "mixr/base/IObject.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"

#include "mixr/base/units/util/angle_utils.hpp"
#include "mixr/base/units/util/length_utils.hpp"

#include <array>

namespace mixr {
namespace models {
class RfEmission;
class IrQueryMsg;
class IPlayer;

//------------------------------------------------------------------------------
// Class: Track
// Factory name: Track
// Description: Generic Target Track
//------------------------------------------------------------------------------
class Track : public base::IObject
{
    DECLARE_SUBCLASS(Track, base::IObject)

public:
    enum TypeBits {
       AIR_TRACK_BIT       = 0x0001,      // Air Track
       GND_TRACK_BIT       = 0x0002,      // Ground Track
       RWR_TRACK_BIT       = 0x0004,      // Radar Warning Receiver Track
       ONBOARD_SENSOR_BIT  = 0x0100,      // Track from an on-board sensor
       DATALINK_BIT        = 0x0200       // Off-board (datalink) track
    };
    enum IffCode { UNKNOWN, FRIENDLY, FOE, COMMERCIAL, OTHER };

    enum TrackClass { NOT_DEFINED, RANGE_AND_ANGLE, ANGLE_ONLY };

public:
   Track();

   // Local track ID
   int getTrackID() const                          { return id; }
   virtual bool setTrackID(const int v);

   // Track type (see TypeBits above)
   bool isType(const short t) const                { return ((type & t) != 0); }
   short getType() const                           { return type; }
   virtual bool setType(const short t);
   virtual bool setSubtype(const short t);

   // Track class (see TypeClass above)
   bool isClass(const TrackClass c) const          { return ((type & c) != 0); }
   TrackClass getClass() const                     { return trackClass; }
   virtual bool setClass(const TrackClass t);

   bool isAngleOnly() const                        { return (trackClass == ANGLE_ONLY); }
   bool isRangeAndAngle() const                    { return (trackClass == RANGE_AND_ANGLE); }

   // Age of the track (seconds) since last update
   double getTrackAge() const                             { return age; }
   virtual bool updateTrackAge(const double dt);
   virtual bool resetTrackAge();

   // Quality of the Track (normalized: high(1.0) to low(0.0)
   double getQuality() const                              { return quality; }
   virtual bool setQuality(const double v);

   // Track position is in meters, NED and centered at ownship.
   const base::Vec3d& getPosition() const                 { return pos; }
   virtual bool setPosition(const base::Vec3d& p);
   virtual bool setPosition(const base::Vec4d& p);

   // Track's estimated position error, meters
   double getCircularError() const                        { return cErr; }
   virtual bool setCircularError(const double err);

   // Track's estimated vertical (altitude) error, meters
   double getVerticalError() const                        { return vErr; }
   virtual bool setVerticalError(const double err);

   // Tracks position (lat/lon)
   bool isLatLonPositionValid() const              { return llValid; }
   bool getLatLonPosition(double* const lat, double* const lon) const;
   virtual bool setLatLonPosition(const double lat, const double lon);
   virtual void clearLatLonValid();

   // Line-Of-Sight (LOS) vector; ownship to track
   const base::Vec3d& getLosVec() const            { return los; }

   // Range to track (meters)
   double getRange() const                         { return rng; }
   double getGroundRange() const                   { return gndRng; }
   virtual bool setRange(const double r);

   // Range rate: ownship-track closer rate, meters/sec, with positive
   // rate for increasing range to track.
   double getRangeRate() const                     { return rngRate; }
   virtual bool setRangeRate(const double rr);

   // Azimuth angle, ownship->track, true north
   double getTrueAzimuth() const                   { return taz; }
   double getTrueAzimuthR() const                  { return taz; }
   double getTrueAzimuthD() const                  { return static_cast<double>(base::angle::R2DCC * taz); }

   // Azimuth angle, ownship->track, relative to ownship
   double getRelAzimuth() const                    { return raz[0]; }
   double getRelAzimuthR() const                   { return raz[0]; }
   double getRelAzimuthD() const                   { return static_cast<double>(base::angle::R2DCC * raz[0]); }
   void setRelAzimuth(double az)                   { raz[0] = az; }

   // Azimuth angle rate, ownship->track, relative to ownship
   double getRelAzimuthRate() const                { return raz[1]; }
   void setRelAzimuthRate(double azRate)           { raz[1] = azRate; }

   // Azimuth angle acceleration, ownship->track, relative to ownship
   double getRelAzimuthAcceleration() const        { return raz[2]; }
   void setRelAzimuthAcceleration(double azAccel)  { raz[2] = azAccel; }

   // Elevation angle, ownship->track, relative to ownship
   double getElevation() const                     { return rel[0]; }
   double getElevationR() const                    { return rel[0]; }
   double getElevationD() const                    { return static_cast<double>(base::angle::R2DCC * rel[0]); }

   void setElevation(double el)                    {rel[0] = el; }

   // Elevation angle rate, ownship->track, relative to ownship
   double getElevationRate() const                 { return rel[1]; }
   void setElevationRate(double elRate)            { rel[1] = elRate; }

   // Elevation angle acceleration, ownship->track, relative to ownship
   double getElevationAcceleration() const         { return rel[2]; }
   void setElevationAcceleration(double elAccel)   { rel[2] = elAccel; }

   // Azimuth angle, ownship->track, relative to ownship
   double getPredictedAzimuth() const              { return predictedRaz[0]; }
   double getPredictedAzimuthR() const             { return predictedRaz[0]; }
   double getPredictedAzimuthD() const             { return static_cast<double>(base::angle::R2DCC * predictedRaz[0]); }

   void setPredictedAzimuth(double az)             { predictedRaz[0] = az; }

   // Azimuth angle rate, ownship->track, relative to ownship
   double getPredictedAzimuthRate() const          { return predictedRaz[1]; }
   void setPredictedAzimuthRate(double azRate)     { predictedRaz[1] = azRate; }

   // Elevation angle, ownship->track, relative to ownship
   double getPredictedElevation() const            { return predictedRel[0]; }
   double getPredictedElevationR() const           { return predictedRel[0]; }
   double getPredictedElevationD() const           { return static_cast<double>(base::angle::R2DCC * predictedRel[0]); }

   void setPredictedElevation(double el)           {predictedRel[0] = el; }

   // Elevation angle rate, ownship->track, relative to ownship
   double getPredictedElevationRate() const        { return predictedRel[1]; }
   void setPredictedElevationRate(double elRate)   { predictedRel[1] = elRate; }

   // Track's estimated ground speed (m/s)
   double getGroundSpeed() const                   { return gndSpd; }
   double getGroundSpeedFPS() const                { return getGroundSpeed() * static_cast<double>(base::length::M2FT); }
   double getGroundSpeedKts() const                { return getGroundSpeed() * static_cast<double>(base::length::M2NM * 3600.0); }

   // Ground track angle, true north
   double getGroundTrack() const                   { return gndTrk; }
   double getGroundTrackR() const                  { return gndTrk; }
   double getGroundTrackD() const                  { return static_cast<double>(base::angle::R2DCC * gndTrk); }

   // Ground track angle, relative to ownship
   double getRelGroundTrack() const                { return relGndTrk; }
   double getRelGroundTrackR() const               { return relGndTrk; }
   double getRelGroundTrackD() const               { return static_cast<double>(base::angle::R2DCC * relGndTrk); }

   // Track->ownship aspect angle
   double getAspectAngle() const                   { return aa; }
   double getAspectAngleR() const                  { return aa; }
   double getAspectAngleD() const                  { return static_cast<double>(base::angle::R2DCC * aa); }

   // Track velocity vector
   const base::Vec3d& getVelocity() const          { return vel; }
   virtual bool setVelocity(const base::Vec3d v);

   // Track acceleration vector
   const base::Vec3d& getAcceleration() const      { return accel; }
   virtual bool setAcceleration(const base::Vec3d a);

   // IFF information
   bool isIffCode(const IffCode s) const           { return (s == iffCode); }
   bool isNotIffCode(const IffCode s) const        { return (s != iffCode); }
   IffCode getIffCode() const                      { return iffCode; }
   virtual bool setIffCode(const IffCode s);

   // Shootlist support
   int getShootListIndex() const                   { return shootList; }
   virtual bool setShootListIndex(const int idx);

   bool isWeaponReleased() const                   { return wpnRel; }
   virtual bool setWeaponReleased(const bool f);

   bool isRejected() const                         { return rejected; }
   virtual bool setRejected(const bool f);

   // OwnshipDynmaics() will apply ownship movements to the track's
   // predicted position, range and velocity.
   virtual void ownshipDynamics(const double gtrk, const base::Vec3d vel, const base::Vec3d accel, const double dt);

   // Optional pointer to the track target player
   IPlayer* getTarget()                            { return tgt; }
   const IPlayer* getTarget() const                { return tgt; }
   virtual bool setTarget(IPlayer* const p);

   // Clear track
   virtual void clear();

protected:
   // General track ID and status
   int         id {};              // Track id;
   short       type {};            // Track type: the bit-wise OR of various type bits (see enum above)
   IffCode     iffCode {UNKNOWN};  // Friend or Foe (see above)
   TrackClass  trackClass;         // Track class (see above) [RF or IR]
   double      age {};             // Track age                               (sec)
   double      quality {};         // Quality of Track; normalized: high(1.0) to low(0.0)

   // Track State Vector
   double      latitude {};       // Latitude                                (degs)
   double      longitude {};      // Longitude                               (degs)
   base::Vec3d los;               // Line-of-sight unit vector (NED)
   base::Vec3d pos;               // position; relative to ownship (NED)     (m)
   base::Vec3d vel;               // Velocity; relative to ownship (NED)     (m/s)
   base::Vec3d accel;             // Acceleration; relative to ownship (NED) ((m/s)/s)
   double      rng {};            // Range from ownship                      (m)
   double      rngRate {};        // Closure rate                            (m/s)
   bool        llValid;           // Latitude & Longitude are valid
   double      cErr {};           // Circular position error                 (m)
   double      vErr {};           // Vertical (altitude) error               (m)

   // Data computed from state vector
   double      raz[3] {};         // Relative azimuth angle (ownship 2 track)  (r)
   double      rel[3] {};         // Relative elevation angle (ownship 2 trk)  (r)
   double      predictedRaz[3];   // Predicted Relative azimuth angle (ownship 2 track)  (r)
   double      predictedRel[3];   // Predicted Relative elevation angle (ownship 2 trk)  (r)
   double      gndRng {};         // Ground Range                              (m)
   double      taz {};            // True azimuth angle (ownship to track)     (r)
   double      aa {};             // Aspect angle                              (r)
   double      gndSpd {};         // Ground speed                              (m/s)
   double      gndTrk {};         // Ground track angle                        (r)
   double      relGndTrk {};      // Ground track angle relative to ownship    (r)

   // Shoot list support
   int         shootList {};      // Shoot list index
   bool        wpnRel {};         // Released weapon on this track
   bool        rejected {};       // Target Rejected (from shoot list)

   // Ownship data
   double      osGndTrk {};       // Ownship's ground track angle             (r)
   base::Vec3d  osVel;            // Ownship velocity (NED)                   (m/s)
   base::Vec3d  osAccel;          // Ownship accel (NED)                      ((m/s)/s)

   IPlayer*    tgt {};            // (Optional) pointer to the Track Player

   bool        mslWarn {};         // Missile warning flag
   static const int MAX_SIG{4};
   std::array<double, MAX_SIG> lastSN{}; // Last MAX_SIG signal values         (dB)
   double      avgSig {};                // Average signal                     (dB)
   double      maxSig {};                // Max Signal                         (dB)
   int         nSig {};                  // Number of signals
   int         iSig {};                  // Signal index;
};


//------------------------------------------------------------------------------
// Class: RfTrack
// Factory name: RfTrack
// Description: Generic R/F Target Track (onboard sensors)
//------------------------------------------------------------------------------
class RfTrack : public Track
{
   DECLARE_SUBCLASS(RfTrack, Track)

public:
   RfTrack();

   // Signal strength and last emission for onboard R/F sensor tracks
   double getAvgSignal() const                     { return avgSig; }
   double getMaxSignal() const                     { return maxSig; }
   int getNumSignals() const                       { return nSig; }
   const RfEmission* getLastEmission() const       { return lastEM; }
   virtual bool setSignal(const double snDbl, const RfEmission* const em);

   // Missile warning (from an RWR)
   bool isMissileWarning() const                   { return mslWarn; }
   virtual bool setMissileWarning(const bool);

   void clear() override;

   virtual bool setLastEmission(const RfEmission* const);

protected:
   const RfEmission* lastEM{};      // Last emission

};

//------------------------------------------------------------------------------
// Class: IrTrack
// Factory name: IrTrack
// Description: Generic IR Target Track (onboard sensors)
//------------------------------------------------------------------------------
class IrTrack : public Track
{
   DECLARE_SUBCLASS(IrTrack, Track)

public:
   IrTrack();

   // Signal strength and last emission for onboard IR sensor tracks
   double getAvgSignal() const                                             { return avgSig; }
   double getMaxSignal() const                                             { return maxSig; }
   int getNumSignals() const                                               { return nSig; }
   const IrQueryMsg* getLastQuery() const                                  { return lastQuery; }
   virtual bool setSignal(const double snDbl, const IrQueryMsg* const);
   bool setPosition(const base::Vec3d&) override;

   // Missile warning (from an RWR)
   bool isMissileWarning() const                                           { return mslWarn; }
   virtual bool setMissileWarning(const bool);

   void clear() override;

//protected:
   virtual bool setLastQuery(const IrQueryMsg* const);

protected:
   const IrQueryMsg* lastQuery {}; // Last query

};

}
}

#endif

