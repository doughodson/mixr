
#ifndef __mixr_models_common_SensorMsg_HPP__
#define __mixr_models_common_SensorMsg_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace models {
class Player;
class IGimbal;

//------------------------------------------------------------------------------
// Class: SensorMsg
// Description: Generic sensor message class; used to pass requests data
//              between sensors (i.e., antennas, IR seekers) and players
//
// Factory name: SensorMsg
//------------------------------------------------------------------------------
class SensorMsg : public base::IObject
{
   DECLARE_SUBCLASS(SensorMsg, base::IObject)

public:
   SensorMsg();

   // ---
   // Normalized ownship to target LOS vector (ownship's NED)
   // ---
   const base::Vec3d& getLosVec() const          { return losO2T; }
   void setLosVec(const base::Vec3d& v)          { losO2T = v; }

   // ---
   // Normalized target to ownship LOS vector (target's NED)
   // ---
   const base::Vec3d& getTgtLosVec() const       { return losT2O; }
   void setTgtLosVec(const base::Vec3d& v)       { losT2O = v; }

   // ---
   // Angles to target in gimbal coordinates
   // ---

   // Azimuth (radians)
   double getGimbalAzimuth() const              { return gaz; }
   void setGimbalAzimuth(const double a)        { gaz = a; }

   // Elevation (radians)
   double getGimbalElevation() const            { return gel; }
   void setGimbalElevation(const double a)      { gel = a; }

   // ---
   // Ranges
   // ---

   // Range from gimbal to target along LOS vector (meters)
   double getRange() const                      { return rng; }

   // Max range of this sensor/message (NM)
   double getMaxRangeNM() const                 { return maxRng; }

   // Sets the range to the target (meters)
   virtual void setRange(const double r);

   // Sets the max range (NM)
   void setMaxRangeNM(const double r)           { maxRng = r; }

   // ---
   // Range rates (projected along the LOS vector)
   // ---

   // Range rate (player to target) along the LOS vector (m/s)
   double getRangeRate() const                  { return rngRate; }

   // Range rate (player to target) along the LOS vector (ft/s)
   double getRangeRateFPS() const;

   // Range rate (player to target) along the LOS vector (knots)
   double getRangeRateKts() const;

   // Sets the target range rate (positive for increasing range) (m/s)
   void setRangeRate(const double v)            { rngRate = v; }

   // ---
   // Target's Angle Of Incidence (AOI); computed by the target
   // player upon receiving this sensor message
   // ---

   // Target's azimuth AOI (radians; target body coordinates)
   double getAzimuthAoi() const                 { return iaz; }

   // Target's elevation AOI (radians; target body coordinates)
   double getElevationAoi() const               { return iel; }

   // Target's normalized AOI vector
   const base::Vec3d& getAoiVector() const      { return aoi; }

   // Sets the target's azimuth AOI (radians; target body coordinates)
   void setAzimuthAoi(const double a)           { iaz = a; }

   // Sets the target's elevation AOI (radians; target body coordinates)
   void setElevationAoi(const double a)         { iel = a; }

   // Sets the target's normalized AOI vector
   void setAoiVector(const base::Vec3d& v)      { aoi = v; }
   void setAoiVector(const base::Vec4d& v)      { aoi.set(v.x(),v.y(),v.z()); }

   // ---
   // Modes and flags
   // ---

   // Process by local players only flag
   bool isLocalPlayersOnly() const              { return localOnly; }

   // Return message requested flag
   // (tells the target player to return this message with its signature)
   bool isReturnRequested() const               { return returnReq; }

   // Sets the 'process only local players' flag
   void setLocalPlayersOnly(const bool b)       { localOnly = b;   }

   // Sets the return request flag
   void setReturnRequest(const bool b)          { returnReq = b; }

   // ---
   // Pointers
   // ---

   // Pointer to the gimbal (seeker, antenna) that sent this message
   IGimbal* getGimbal()                         { return gimbal; }
   const IGimbal* getGimbal() const             { return gimbal; }

   // Pointer to the player that sent this message
   Player* getOwnship()                         { return ownship; }
   const Player* getOwnship() const             { return ownship; }

   // Pointer to the target of this message
   Player* getTarget()                          { return target; }
   const Player* getTarget() const              { return target; }

   // Optional: data message attached to sensor message
   base::IObject* getDataMessage()              { return dataMsg; }
   const base::IObject* getDataMessage() const  { return dataMsg; }

   // Sets the gimbal that generated this message
   void setGimbal(IGimbal* const);

   // Sets the player that sent this message
   void setOwnship(Player* const);

   // Sets the target player
   void setTarget(Player* const);

   // Sets the optional data message attached to sensor message
   void setDataMessage(base::IObject* const);

   // Clear data
   virtual void clear();

private:
   void initData();

   double maxRng {};         // Maximum range                                (NM)
   double rng {};            // Range                                        (meters)
   double rngRate {};        // Range Rate                                   (m/s)
   double gaz {};            // Gimbal azimuth                               (rad)
   double gel {};            // Gimbal elevation                             (rad)
   double iaz {};            // Target's Angle Of Incidence (AOI) azimuth    (rad)
   double iel {};            // Target's AOI elevation                       (rad)
   base::Vec3d losO2T;       // Normalized ownship to target LOS vector (ownship's NED)
   base::Vec3d losT2O;       // Normalized target to ownship LOS vector (target's NED)
   base::Vec3d aoi;          // Normalized target Angle Of Incidence (AOI) vector
   IGimbal* gimbal {};       // The gimbal that transmitted the message
   base::safe_ptr<Player> ownship;         // The originating (ownship) player
   base::safe_ptr<Player> target;          // The Target player
   base::safe_ptr<base::IObject> dataMsg;  // Embedded data message (e.g., datalink, etc)
   bool returnReq {};        // Return Request
   bool localOnly {};        // Local players only flag
};

}
}

#endif
