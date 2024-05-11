
#ifndef __mixr_models_common_Gimbal_HPP__
#define __mixr_models_common_Gimbal_HPP__

#include "mixr/models/common/system/System.hpp"

#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Matrixd"

namespace mixr {
namespace base { class Angle; class Boolean; class Identifier; class Integer; class Length; class List; class Number; class PairStream; }
namespace models {
class Emission;
class SensorMsg;
class Tdb;

//------------------------------------------------------------------------------
// Class: Gimbal
//
// Description: Generic class for all gimbals, RF antennas, IR seekers, etc
//
// Notes:
//    1) This class is one of the "top level" systems attached to a Player
//    class (see Player.hpp).
//
//    2) By convention, R/F and IR systems are responsible for filtering their
//    own players of interest.  This gimbal class has its own variation of the
//    processPlayersOfInterest() function that filters field of view and player
//    type.  However, it is still the responsibility of the systems to use
//    or not use our member function.
//
//    3) Gimbal coordinates:
//       X+ is along the gimbal/sensor boresight
//       Y+ is to the right of the gimbal boresight
//       Z+ is down
//
//    4) The matrix (see below) is a composite of this gimbal's angles and
//    all of our container gimbals' angles.  Therefore, this matrix transforms
//    vectors to(from) our player's body coordinate and not to(from) our container
//    gimbal's coordinate system.
//
//          Usage:
//             Vg = M * Vb
//             Vb = Vg * M
//          Where:
//             M = Rx[roll] * Ry[pitch] * Rz[yaw] * T[xyz] * M[container]
//             Vg is a vector in gimbal coordinates
//             Vb is a vector in body coordinates
//
//
//
//
// Factory name: Gimbal
// Slots:
//
//    type                 <Identifier>   ! Physical identifiers: { mechanical, electronic } (default: electronic)
//
//    location             <List>         ! Relative location vector (meters) [ x y z ] (default: 0,0,0)
//
//    initPosition         <List>         ! Initial position vector (radians) [ az el roll ] (default: 0,0,0)
//    initPosAzimuth       <Angle>        ! Initial azimuth position (default: 0)
//    initPosElevation     <Angle>        ! Initial elevation position (default: 0)
//    initPosRoll          <Angle>        ! Initial roll position (default: 0)
//
//    azimuthLimits        <List>         ! Lower & upper azimuth limits (radians) [ left right ][ range: -pi to pi; else unlimited ]
//    azimuthLimitLeft     <Angle>        ! Left azimuth limit[ range: -pi to pi; else unlimited ]
//    azimuthLimitRight    <Angle>        ! Right azimuth limit[ range: -pi to pi; else unlimited ]
//
//    elevationLimits      <List>         ! Lower & upper elevation limits (radians) [ lower upper ][ range: -pi to pi; else unlimited ]
//    elevationLimitLower  <Angle>        ! Lower elevation limit[ range: -pi to pi; else unlimited ]
//    elevationLimitUpper  <Angle>        ! Upper elevation limit[ range: -pi to pi; else unlimited ]
//
//    rollLimits           <Angle>        ! Roll limit vector (radians) [ lower upper ][ range: -pi to pi; else unlimited ]
//    rollLimitLower       <Angle>        ! Lower roll limit[ range: -pi to pi; else unlimited ]
//    rollLimitUpper       <Angle>        ! Upper roll limit[ range: -pi to pi; else unlimited ]
//
//    maxRates             <List>         ! Max mechanical rate vector (rad/sec) [ az el roll ]
//    maxRateAzimuth       <Angle>        ! Max "mechanical" azimuth rate    (Angle/sec) (default: D2RCC * 120.0)
//    maxRateElevation     <Angle>        ! Max "mechanical" elevation rate  (Angle/sec) (default: D2RCC * 120.0)
//    maxRateRoll          <Angle>        ! Max "mechanical" roll rate       (Angle/sec) (default: D2RCC * 120.0)
//
//    commandPosition      <List>         ! Commanded position vector (radians) [ az el roll ] (sets POSITION_SERVO)
//    commandPosAzimuth    <Angle>        ! Commanded azimuth position    (sets POSITION_SERVO) (default: 0)
//    commandPosElevation  <Angle>        ! Commanded elevation position  (sets POSITION_SERVO) (default: 0)
//    commandPosRoll       <Angle>        ! Commanded roll position       (sets POSITION_SERVO) (default: 0)
//
//    commandRates         <List>         ! Commanded rate vector (rad/sec) [ az el roll ] (sets RATE_SERVO)
//    commandRateAzimuth   <Angle>        ! Commanded azimuth rate     (Angle/sec) (sets RATE_SERVO)
//    commandRateElevation <Angle>        ! Commanded elevation rate  (Angle/sec) (sets RATE_SERVO)
//    commandRateRoll      <Angle>        ! Commanded roll rate        (Angle/sec) (sets RATE_SERVO)
//
//    terrainOcculting     <Boolean>      ! Enable terrain occulting of the players of interest (default: false)
//    checkHorizon         <Boolean>      ! Enable horizon masking check (default: true)
//
//    playerOfInterestTypes        <PairStream> ! List of player of interest types (default: all types )
//                                              ! Valid identifiers: { air, ground, weapon, ship, building, lifeform, space }
//
//    maxPlayersOfInterest         <Number>     ! Max number of players of interest (default: 200)
//    maxRange2PlayersOfInterest   <Length>     ! Max range to players of interest, or zero for all (default: 0)
//    maxAngle2PlayersOfInterest   <Angle>      ! Max angle off the gimbal boresight to players of interest, or zero for all (default: 0)
//    localPlayersOfInterestOnly   <Boolean>    ! Sets the local only players of interest flag (default: false)
//
//    useWorldCoordinates          <Boolean>    ! Using player of interest's world (ECEF) coordinate system (default: true)
//    useOwnHeadingOnly            <Boolean>    ! Whether only the ownship heading is used by the target data block (default: true)
//
//
// Events:
//    RF_EMISSION                  <Emission>   ! Default handler: Pass emissions to subcomponents.
//
//
//  Handy support functions
//      limitVec(base::Vec2d& vec, base::Vec2d& lim)
//      limitVec(base::Vec2d& vec, base::Vec2d& ll, base::Vec2d& ul)
//          Symmetrical and asymmetrical limit checks
//
//------------------------------------------------------------------------------
class Gimbal : public System
{
   DECLARE_SUBCLASS(Gimbal, System)

public:

   // Servo type
   enum class Type {
      MECHANICAL,   // Mechanical gimbal
      ELECTRONIC    // Electronic servo (e.g., phased array)
   };

   // Servo Mode
   enum class ServoMode {
      FREEZE,         // Servo is frozen
      RATE,           // Rate servo
      POSITION        // Position servo
   };

   // Position/velocity vector indexes
   enum {
      AZ_IDX,    // Azimuth index
      ELEV_IDX,  // Elevation index
      ROLL_IDX   // Roll index
   };

   // Max number of players of interest
   enum { MAX_PLAYERS = MIXR_CONFIG_MAX_PLAYERS_OF_INTEREST };

public:  // Public section
   Gimbal();

   // Returns 'true' if servo is of type 'tt'.
   bool isGimbalType(const Type tt) const          { return (tt == type); }

   // Returns the type of servo: { MECHANICAL, ELECTRONIC }
   Type getGimbalType() const                      { return type; }

   // Returns the servo mode: { FREEZE_SERVO, RATE_SERVO, POSITION_SERVO }
   ServoMode getServoMode() const                  { return servoMode; }

   // Returns the current position vector [ az el roll ] (radians)
   const base::Vec3d& getPosition() const          { return pos; }

   double getAzimuth() const     { return pos[AZ_IDX]; }    // Returns current azimuth position (rad)
   double getElevation() const   { return pos[ELEV_IDX]; }  // Returns current elevation position (rad)
   double getRoll() const        { return pos[ROLL_IDX]; }  // Returns current roll position (rad)
   double getAzimuthD() const;                              // Returns current azimuth position (deg)
   double getElevationD() const;                            // Returns current elevation position (deg)
   double getRollD() const;                                 // Returns current roll position (deg)

   // Returns true if the gimbal position is at the commanded position,
   // with a position tolerance of 'tol' radians.  The default tolerance
   // is 1/10th of a degree.
   virtual bool isPositioned(const double tol = -1) const;

   // Returns true if the gimbal is at a 'physical' limit
   virtual bool isAtLimits() const;

   const base::Vec3d& getRates() const { return rate; }        // Returns the current rate vector (rad/sec)
   double getAzimuthRate() const    { return rate[AZ_IDX]; }   // Returns current azimuth rate (rad/sec)
   double getElevationRate() const  { return rate[ELEV_IDX]; } // Returns current elevation rate (rad/sec)
   double getRollRate() const       { return rate[ROLL_IDX]; } // Returns current roll rate (rad/sec)

   bool isFastSlewMode() const      { return fastSlew; }       // Returns true if the servo is in the 'fast' slewing mode.
   bool isSlowSlewMode() const      { return !fastSlew; }      // Returns true if the servo is in the 'slow' slewing mode.

   const base::Vec3d& getCmdPosition() const { return cmdPos; }      // Returns the commanded position vector [ az el roll ] (radians)
   double getCmdAz() const          { return cmdPos[AZ_IDX]; }       // Returns commanded azimuth position (rad)
   double getCmdElev() const        { return cmdPos[ELEV_IDX]; }     // Returns commanded elevation position (rad)
   double getCmdRoll() const        { return cmdPos[ROLL_IDX]; }     // Returns commanded roll position (rad)

   const base::Vec3d& getCmdRates() const { return cmdRate; }        // Returns the current rate vector (rad/sec)
   double getCmdAzRate() const         { return cmdRate[AZ_IDX]; }   // Returns commanded azimuth rate (rad/sec)
   double getCmdElevRate() const       { return cmdRate[ELEV_IDX]; } // Returns commanded elevation rate (rad/sec)
   double getCmdRollRate() const       { return cmdRate[ROLL_IDX]; } // Returns commanded roll rate (rad/sec)

   const base::Vec3d& getLocation() const  { return location; }      // Returns the location vector of the gimbal on its parent container  (meters)
   const base::Matrixd& getRotMat() const  { return tm; }            // Returns the gimbal's rotational matrix
                                                                     //    body (nose/right/down) <== > this.gimbal(x,y,z)
                                                                     //    Usage:
                                                                     //       Vg = M * Vb
                                                                     //       Vb  = Vg * M
                                                                     //    Where:
                                                                     //       M = Rx[roll] * Ry[pitch] * Rz[yaw] * T[xyz] * M[container]
                                                                     //       Vg is a vector in gimbal coordinates
                                                                     //       Vb is a vector in body coordinates

   void getAzimuthLimits(double* const leftLim, double* const rightLim) const;   // Returns the physical azimuth limits (rad)
   void getElevationLimits(double* const lowerLim, double* const upperLim) const;// Returns the physical elevation limits (rad)
   void getRollLimits(double* const lowerLim, double* const upperLim) const;     // Returns the physical roll limits (rad)

   // Returns the maximum mechanical rates (rad/sec)
   void getMaxRates(double* const azMaxRate, double* const ezMaxRate, double* const rollMaxRate) const;

   double getMaxRange2PlayersOfInterest() const { return maxRngPlayers; }   // Max range to players of interest or zero for all (meters)
   double getMaxAngle2PlayersOfInterest() const { return maxAnglePlayers; } // Max angle of gimbal boresight to players of interest or zero for all (rad)
   unsigned int getPlayerOfInterestTypes() const { return playerTypes; }    // Player of interest types (Player::MajorType bit-wise or'd)
   unsigned int getMaxPlayersOfInterest() const  { return maxPlayers; }     // Max number of players of interest (i.e., size of the arrays)
   bool isLocalPlayersOfInterestOnly() const { return localOnly; }          // Local only players of interest flag
   bool isTerrainOccultingEnabled() const  { return terrainOcculting; }     // Terrain occulting enabled flag
   bool isHorizonCheckEnabled() const      { return checkHorizon; }         // Horizon masking enable flag
   bool isUsingWorldCoordinates() const    { return useWorld; }             // Returns true if using player of interest's world coordinates
   bool isUsingHeadingOnly() const         { return ownHeadingOnly; }       // Returns true if using players heading only
   double getEarthRadius() const;                                           // Returns earth radius (meters)
   virtual bool fromPlayerOfInterest(const Emission* const em);             // Returns true if this emission is from a player of interest

   virtual bool setMaxRange2PlayersOfInterest(const double meters);        // Max range to players of interest or zero for all (meters)
   virtual bool setMaxAngle2PlayersOfInterest(const double radians);       // Max angle of gimbal boresight to players of interest or zero for all (rad)
   virtual bool setPlayerOfInterestTypes(const unsigned int typeMask);     // Player of interest types (Player::MajorType bit-wise or'd)
   virtual bool setMaxPlayersOfInterest(const unsigned int);               // Max number of players of interest (i.e., size of the arrays)
   virtual bool setLocalPlayersOfInterestOnly(const bool);                 // Sets the local only players of interest flag
   virtual bool setTerrainOccultingEnabled(const bool);                    // Sets the terrain occulting enabled flag
   virtual bool setHorizonCheckEnabled(const bool);                        // Sets the horizon check enabled flag
   virtual bool setUseWorld(const bool);                                   // Sets the using world coordinates flag
   virtual bool setOwnHeadingOnly(const bool);                             // Use only the ownship player's heading to when transforming between body and local NED

   // Process the Players-Of-Interest (POI) list
   virtual unsigned int processPlayersOfInterest(base::PairStream* const poi);

   // Sets the servo mode: { FREEZE_SERVO, RATE_SERVO, POSITION_SERVO }
   // Returns false if the mode could not be changed
   virtual bool setServoMode(const ServoMode);

   virtual bool setPosition(const double azim, const double elev);                     // Sets the current az & el positions (rad)
   virtual bool setPosition(const double azim, const double elev, const double roll);  // Sets the current az, el and roll positions (rad)

   virtual bool setMaxRates(const double azMaxRate, const double ezMaxRate);           // Sets the max mechanical az & el rates (rad/sec)
   virtual bool setMaxRates(const double azMaxRate, const double ezMaxRate, const double rollMaxRate);   // Sets the max mechanical az, el and roll rates (rad/sec)

   virtual bool setCmdPos(const base::Vec2d& p);                                       // Sets the commanded position vector (rad)
   virtual bool setCmdPos(const base::Vec3d& p);                                       // Sets the commanded position vector (rad)
   virtual bool setCmdPos(const double az, const double el, const double roll = 0);    // Sets the commanded az, el and roll positions (rad)

   virtual bool setCmdRate(const base::Vec2d& r);                                      // Sets the commanded rate vector (rad/sec)
   virtual bool setCmdRate(const base::Vec3d& r);                                      // Sets the commanded rate vector (rad/sec)
   virtual bool setCmdRate(const double azRate, const double elRate);                  // Sets the commanded az & el rates (rad/sec)
   virtual bool setCmdRate(const double azRate, const double elRate, const double rollRate); // Sets the commanded az, el and roll rates (rad/sec)

   virtual bool setAzimuthLimits(const double leftLim, const double rightLim);         // Sets the mechanical azimuth limits (rad)
   virtual bool setElevationLimits(const double lowerLim, const double upperLim);      // Sets the mechanical elevation limits (rad)
   virtual bool setRollLimits(const double lowerLim, const double upperLim);           // Sets the mechanical roll limits (rad)

   virtual bool setLocation(const double x, const double y, const double z);           // Sets the location vector of the gimbal on its parent container (meters)

   virtual bool setFastSlewMode(const bool);                                           // Sets the servo 'fast' or 'slow' slewing mode


   // Event handler(s)
   virtual bool onRfEmissionEvent(Emission* const);                             // Handles R/F emission events

   static void limitVec(base::Vec2d& vec, const base::Vec2d& lim);
   static void limitVec(base::Vec3d& vec, const base::Vec3d& lim);
   static void limitVec(base::Vec2d& vec, const base::Vec2d& ll, const base::Vec2d& ul);
   static void limitVec(base::Vec3d& vec, const base::Vec3d& ll, const base::Vec3d& ul);

   bool event(const int event, base::Object* const obj = nullptr) override;
   void reset() override;

protected:
   virtual void servoController(const double dt = 0.0);
   virtual bool setType(const Type rt);
   virtual void updateMatrix();

   Tdb* getCurrentTDB();               // Get the current TDB (pre-ref())
   const Tdb* getCurrentTDB() const;   // Get the current TDB (pre-ref(); const version)

   // Set the current TDB
   bool setCurrentTdb(Tdb* const);

   // System Interface -- Callbacks by phase
   void dynamics(const double dt) override;    // Phase 0

   // base::Component protected interface
   bool shutdownNotification() override;

private:
   void initData();

   static const double defaultTolerance;

   Type        type{Type::ELECTRONIC};         // Mechanical or Electronic gimbal (affects maxRates)
   ServoMode   servoMode{ServoMode::FREEZE};   // Gimbal's servo mode
   bool        fastSlew{true};                 // Fast slewing mode: tell us if we are slewing fast (true) or scanning (slewing slow (false))

   base::Matrixd tm;                  // Transformation matrix (to/from the player's coordinate system)
   base::Vec3d  pos;                  // Current gimbal position      (rad)
   base::Vec3d  rate;                 // Current velocity             (rad/sec)
   base::Vec3d  cmdPos;               // Commanded position           (rad)
   base::Vec3d  cmdRate;              // Commanded rate               (rad/sec)
   base::Vec3d  location;             // Gimbal's location on parent  (meters)
   bool atLimit{};                    // Gimbal is at a limit

   base::Vec3d  maxRate;              // Max mechanical rate of gimbal (rad/sec)
   base::Vec3d  lowLimits;            // left/lower gimbal limits     (rad)
   base::Vec3d  highLimits;           // right/upper gimbal limits    (rad)

   base::Vec3d  initPos;              // Initial gimbal position      (rad)
   base::Vec3d  initCmdPos;           // Initial commanded position   (rad)
   base::Vec3d  initCmdRate;          // Initial commanded rate       (rad/sec)

   double    maxRngPlayers{};         // Max range for players of interest or zero for all (meters)
   double    maxAnglePlayers{};       // Max angle of gimbal boresight for players of interest (or zero for all) (rad)
   unsigned int playerTypes{0xFFFF};  // Player of interest type mask (default: all players)
   unsigned int maxPlayers{200};      // Max number of players of interest (i.e., size of the arrays)
   bool     localOnly{};              // Local players of interest only
   bool     terrainOcculting{};       // Target terrain occulting enabled flag
   bool     checkHorizon{true};       // Horizon masking check enabled flag
   bool     useWorld{true};           // Using player of interest's world coordinates
   bool     ownHeadingOnly{true};     // Whether only the ownship heading is used by the target data block

   base::safe_ptr<Tdb> tdb;           // Target Data Block

private:
   // slot table helper methods
   bool setSlotType(const base::Identifier* const);
   bool setSlotLocation(const base::List* const);

   bool setSlotPosition(const base::List* const);
   bool setSlotPosAzimuth(const base::Angle* const);
   bool setSlotPosElevation(const base::Angle* const);
   bool setSlotPosRoll(const base::Angle* const);

   bool setSlotAzimuthLimits(const base::List* const);
   bool setSlotAzimuthLimitLeft(const base::Angle* const);
   bool setSlotAzimuthLimitRight(const base::Angle* const);

   bool setSlotElevationLimits(const base::List* const);
   bool setSlotElevationLower(const base::Angle* const);
   bool setSlotElevationUpper(const base::Angle* const);

   bool setSlotRollLimits(const base::List* const);
   bool setSlotRollLimitLower(const base::Angle* const);
   bool setSlotRollLimitUpper(const base::Angle* const);

   bool setSlotMaxRates(const base::List* const);
   bool setSlotMaxRateAzimuth(const base::Angle* const);
   bool setSlotMaxRateElevation(const base::Angle* const);
   bool setSlotMaxRateRoll(const base::Angle* const);

   bool setSlotCmdPos(const base::List* const);
   bool setSlotCmdPosAzimuth(const base::Angle* const);
   bool setSlotCmdPosElevation(const base::Angle* const);
   bool setSlotCmdPosRoll(const base::Angle* const);

   bool setSlotCmdRate(const base::List* const);
   bool setSlotCmdRateAzimuth(const base::Angle* const);
   bool setSlotCmdRateElevation(const base::Angle* const);
   bool setSlotCmdRateRoll(const base::Angle* const);

   bool setSlotTerrainOcculting(const base::Boolean* const);
   bool setSlotCheckHorizon(const base::Boolean* const);

   bool setSlotPlayerTypes(const base::PairStream* const);
   bool setSlotMaxPlayers(const base::Integer* const);

   bool setSlotMaxRange2PlayersOfInterest(const base::Length* const);

   bool setSlotMaxAngle2PlayersOfInterest(const base::Angle* const);

   bool setSlotLocalPlayersOfInterestOnly(const base::Boolean* const);

   bool setSlotUseWorldCoordinates(const base::Boolean* const);

   bool setSlotUseOwnHeadingOnly(const base::Boolean* const);
};

}
}

#endif
