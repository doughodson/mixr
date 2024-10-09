
#include "mixr/models/system/Gimbal.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/Emission.hpp"
#include "mixr/models/Tdb.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/angles.hpp"
#include "mixr/base/units/lengths.hpp"

#include "mixr/base/util/nav_utils.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Gimbal, "Gimbal")

BEGIN_SLOTTABLE(Gimbal)
    "type",                   //  1: Physical gimbal type: mechanical or electronic

    "location",               //  2: Relative location vector         (meters) [ x y z ]

    "initPosition",           //  3: Initial position vector          (radians) [ az el roll ]
    "initPosAzimuth",         //  4: Initial azimuth position         (base::Angle)
    "initPosElevation",       //  5: Initial elevation position       (base::Angle)
    "initPosRoll",            //  6: Initial roll position            (base::Angle)

    "azimuthLimits",          //  7: Azimuth limit vector             (radians) [ left right ] [ range: -pi to pi; else unlimited ]
    "azimuthLimitLeft",       //  8: Left azimuth limit               (base::Angle) [ range: -pi to pi; else unlimited ]
    "azimuthLimitRight",      //  9: Right azimuth limit              (base::Angle) [ range: -pi to pi; else unlimited ]

    "elevationLimits",        // 10: Elevation limit vector           (radians) [ lower upper ] [ range: -pi to pi; else unlimited ]
    "elevationLimitLower",    // 11: Lower elevation limit            (base::Angle) [ range: -pi to pi; else unlimited ]
    "elevationLimitUpper",    // 12: Upper elevation limit            (base::Angle) [ range: -pi to pi; else unlimited ]

    "rollLimits",             // 13: Roll limit vector                (radians) [ lower upper ] [ range: -pi to pi; else unlimited ]
    "rollLimitLower",         // 14: Lower roll limit                 (base::Angle) [ range: -pi to pi; else unlimited ]
    "rollLimitUpper",         // 15: Upper roll limit                 (base::Angle) [ range: -pi to pi; else unlimited ]

    "maxRates",               // 16: Max "mechanical" rate vector     (rad/sec) [ az el roll ]
    "maxRateAzimuth",         // 17: Max "mechanical" azimuth rate    (base::Angle/sec)
    "maxRateElevation",       // 18: Max "mechanical" elevation rate  (base::Angle/sec)
    "maxRateRoll",            // 19: Max "mechanical" roll rate       (base::Angle/sec)

    "commandPosition",        // 20: Commanded position vector        (radians) [ az el roll ] (sets POSITION_SERVO)
    "commandPosAzimuth",      // 21: Commanded azimuth position       (base::Angle) (sets POSITION_SERVO)
    "commandPosElevation",    // 22: Commanded elevation position     (base::Angle) (sets POSITION_SERVO)
    "commandPosRoll",         // 23: Commanded roll position          (base::Angle) (sets POSITION_SERVO)

    "commandRates",           // 24: Commanded rate vector            (rad/sec) [ az el roll ] (sets RATE_SERVO)
    "commandRateAzimuth",     // 25: Commanded azimuth rate           (base::Angle/sec) (sets RATE_SERVO)
    "commandRateElevation",   // 26: Commanded elevation rate         (base::Angle/sec) (sets RATE_SERVO)
    "commandRateRoll",        // 27: Commanded roll rate              (base::Angle/sec) (sets RATE_SERVO)

    "terrainOcculting",             // 28: Enable terrain occulting of tye players of interest (default: false)
    "checkHorizon",                 // 29:  Enable horizon masking check (default: true)
    "playerOfInterestTypes",        // 30: Player of interest types (default: 0 )
                                    //     valid identifiers: { air, ground, weapon, ship, building, lifeform, space }
    "maxPlayersOfInterest",         // 31: Max number of players of interest (default: 0)
    "maxRange2PlayersOfInterest",   // 32: Max range to players of interest or zero for all (default: 0)
    "maxAngle2PlayersOfInterest",   // 33: Max angle of gimbal boresight to players of interest or zero for all (default: 0)
    "localPlayersOfInterestOnly",   // 34: Sets the local only players of interest flag (default: false)
    "useWorldCoordinates",          // 35: Using player of interest's world (ECEF) coordinate system
    "ownHeadingOnly",               // 36: Whether only the ownship heading is used by the target data block
END_SLOTTABLE(Gimbal)

BEGIN_SLOT_MAP(Gimbal)

    ON_SLOT(1, setSlotType,                        base::Identifier) // Physical gimbal type: mechanical or electronic

    ON_SLOT(2, setSlotLocation,                    base::List)       // Relative location vector (meters) [ x y z ]

    ON_SLOT(3, setSlotPosition,                    base::List)       // Initial position vector (radians) [ az el roll ]
    ON_SLOT(4, setSlotPosAzimuth,                  base::Angle)      // Initial azimuth position
    ON_SLOT(5, setSlotPosElevation,                base::Angle)      // Initial elevation position
    ON_SLOT(6, setSlotPosRoll,                     base::Angle)      // Initial roll position

    ON_SLOT(7, setSlotAzimuthLimits,               base::List)       // Azimuth limit vector (radians) [ left right ]
    ON_SLOT(8, setSlotAzimuthLimitLeft,            base::Angle)      // Left azimuth limit
    ON_SLOT(9, setSlotAzimuthLimitRight,           base::Angle)      // Right azimuth limit

    ON_SLOT(10, setSlotElevationLimits,            base::List)       // Elevation limit vector (radians) [ lower upper ]
    ON_SLOT(11, setSlotElevationLower,             base::Angle)      // Lower elevation limit
    ON_SLOT(12, setSlotElevationUpper,             base::Angle)      // Upper elevation limit

    ON_SLOT(13, setSlotRollLimits,                 base::List)       // Roll limit vector (radians) [ lower upper ]
    ON_SLOT(14, setSlotRollLimitLower,             base::Angle)      // Lower roll limit
    ON_SLOT(15, setSlotRollLimitUpper,             base::Angle)      // Upper roll limit

    ON_SLOT(16, setSlotMaxRates,                   base::List)       // Max "mechanical" rate vector (rad/sec) [ az el roll ]
    ON_SLOT(17, setSlotMaxRateAzimuth,             base::Angle)      // Max "mechanical" azimuth rate (base::Angle/sec)
    ON_SLOT(18, setSlotMaxRateElevation,           base::Angle)      // Max "mechanical" elevation rate (base::Angle/sec)
    ON_SLOT(19, setSlotMaxRateRoll,                base::Angle)      // Max "mechanical" roll rate (base::Angle/sec)

    ON_SLOT(20, setSlotCmdPos,                     base::List)       // Commanded position vector  [ az el roll ] (sets POSITION_SERVO)
    ON_SLOT(21, setSlotCmdPosAzimuth,              base::Angle)      // Commanded azimuth position  (sets POSITION_SERVO)
    ON_SLOT(22, setSlotCmdPosElevation,            base::Angle)      // Commanded elevation position (sets POSITION_SERVO)
    ON_SLOT(23, setSlotCmdPosRoll,                 base::Angle)      // Commanded roll position  (sets POSITION_SERVO)

    ON_SLOT(24, setSlotCmdRate,                    base::List)       // Commanded rate vector (rad/sec) [ az el roll ] (sets RATE_SERVO)
    ON_SLOT(25, setSlotCmdRateAzimuth,             base::Angle)      // Commanded azimuth rate (sets RATE_SERVO)
    ON_SLOT(26, setSlotCmdRateElevation,           base::Angle)      // Commanded elevation rate (sets RATE_SERVO)
    ON_SLOT(27, setSlotCmdRateRoll,                base::Angle)      // Commanded roll rate (sets RATE_SERVO)

    ON_SLOT(28, setSlotTerrainOcculting,           base::Boolean)    // Enable terrain occulting (default: false)
    ON_SLOT(29, setSlotCheckHorizon,               base::Boolean)    // Enable horizon masking check (default: all types)
    ON_SLOT(30, setSlotPlayerTypes,                base::PairStream) // Player of interest types (default: 0 )
                                                                     //   Valid identifiers: { air, ground, weapon, ship, building, lifeform }
    ON_SLOT(31, setSlotMaxPlayers,                 base::Integer)    // Max number of players of interest (default: 0)
    ON_SLOT(32, setSlotMaxRange2PlayersOfInterest, base::Length)     // Max range to players of interest or zero for all (default: 0)
    ON_SLOT(33, setSlotMaxAngle2PlayersOfInterest, base::Angle)      // Max angle of gimbal boresight to players of interest or zero for all (default: 0)
    ON_SLOT(34, setSlotLocalPlayersOfInterestOnly, base::Boolean)    // Sets the local only players of interest flag (default: false)

    ON_SLOT(35, setSlotUseWorldCoordinates,        base::Boolean)    // Using player of interest's world (ECEF) coordinate system
    ON_SLOT(36, setSlotUseOwnHeadingOnly,          base::Boolean)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Gimbal)
    ON_EVENT_OBJ(RF_EMISSION, onRfEmissionEvent, Emission)
END_EVENT_HANDLER()

const double Gimbal::defaultTolerance{0.1 * (base::PI/180.0)};

Gimbal::Gimbal()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void Gimbal::initData()
{
   pos.set(0,0,0);
   rate.set(0,0,0);
   cmdPos.set(0,0,0);
   location.set(0,0,0);

   cmdRate.set((base::angle::D2RCC *   0.0), (base::angle::D2RCC *   0.0), (base::angle::D2RCC *   0.0));
   maxRate.set((base::angle::D2RCC * 120.0), (base::angle::D2RCC * 120.0), (base::angle::D2RCC * 120.0));

   lowLimits.set( (base::angle::D2RCC * -720.0), (base::angle::D2RCC * -720.0), (base::angle::D2RCC * -720.0));   // default: no limits
   highLimits.set((base::angle::D2RCC *  720.0), (base::angle::D2RCC *  720.0), (base::angle::D2RCC *  720.0));    // default: no limits

   initPos = pos;
   initCmdRate = cmdRate;
   initCmdPos = cmdPos;
}

void Gimbal::copyData(const Gimbal& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   type = org.type;
   servoMode = org.servoMode;

   pos = org.pos;
   rate = org.rate;
   cmdPos = org.cmdPos;
   cmdRate = org.cmdRate;
   maxRate = org.maxRate;
   lowLimits = org.lowLimits;
   highLimits = org.highLimits;
   location = org.location;
   atLimit = org.atLimit;
   fastSlew = org.fastSlew;
   initPos = org.initPos;
   initCmdRate = org.initCmdRate;
   initCmdPos = org.initCmdPos;

   maxRngPlayers = org.maxRngPlayers;
   maxAnglePlayers = org.maxAnglePlayers;
   localOnly = org.localOnly;
   terrainOcculting = org.terrainOcculting;
   checkHorizon = org.checkHorizon;
   useWorld = org.useWorld;
   ownHeadingOnly = org.ownHeadingOnly;
   playerTypes = org.playerTypes;
   maxPlayers = org.maxPlayers;

   tdb = nullptr;
}

void Gimbal::deleteData()
{
   tdb = nullptr;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Gimbal::reset()
{
   pos = initPos;
   cmdRate = initCmdRate;
   cmdPos = initCmdPos;
   updateMatrix();
   BaseClass::reset();
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool Gimbal::shutdownNotification()
{
    tdb = nullptr;

    return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// dynamics() -- System class "Dynamics phase" call back
//------------------------------------------------------------------------------
void Gimbal::dynamics(const double dt)
{
   servoController(dt);
   BaseClass::dynamics(dt);
}

//------------------------------------------------------------------------------
// onRfEmissionEvent() -- process events for RF Emission not sent by us.
//------------------------------------------------------------------------------
bool Gimbal::onRfEmissionEvent(Emission* const em)
{
   if (isComponentSelected()) {
      // Just pass it to our selected subcomponent
      const auto sc = dynamic_cast<Gimbal*>( getSelectedComponent() );
      if (sc != nullptr && sc->getPowerSwitch() != System::PWR_OFF) sc->onRfEmissionEvent(em);
   } else {
      // Pass it down to all of our subcomponents
      base::PairStream* subcomponents = getComponents();
      if (subcomponents != nullptr) {
         for (base::List::Item* item = subcomponents->getFirstItem(); item != nullptr; item = item->getNext()) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto sc = dynamic_cast<Gimbal*>( pair->object() );
            if (sc != nullptr && sc->getPowerSwitch() != System::PWR_OFF) sc->onRfEmissionEvent(em);
         }
         subcomponents->unref();
         subcomponents = nullptr;
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// Returns true if this is a player of interest
//------------------------------------------------------------------------------
bool Gimbal::fromPlayerOfInterest(const Emission* const em)
{
   bool ok{};
   if (em != nullptr) {
      const Player* const p{em->getOwnship()};
      if (p != nullptr) {
         ok = ((playerTypes & p->getMajorType()) != 0);
         if (ok && localOnly) {
            ok = p->isLocalPlayer();
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Returns earth radius (meters)
//------------------------------------------------------------------------------
double Gimbal::getEarthRadius() const
{
   double erad{base::nav::ERAD60 * base::length::NM2M};
   const Player* own{getOwnship()};
   if (own != nullptr) {
      erad = own->getEarthRadius();
   }

   return erad;
}

//------------------------------------------------------------------------------
// servoController() -- Gimbal's core servo model
//------------------------------------------------------------------------------
void Gimbal::servoController(const double dt)
{
   // Only if we're not frozen ...
   if (servoMode != ServoMode::FREEZE) {

      // ---
      // Compute rate
      // ---
      base::Vec3d rate1( 0.0f, 0.0f, 0.0f );
      if (servoMode == ServoMode::POSITION) {

         // position servo: drive the gimbal toward the commanded position
         rate1 = cmdPos - pos;
         rate1[AZ_IDX]   = base::angle::aepcdRad(rate1[AZ_IDX]);
         rate1[ELEV_IDX] = base::angle::aepcdRad(rate1[ELEV_IDX]);
         rate1[ROLL_IDX] = base::angle::aepcdRad(rate1[ROLL_IDX]);

         // ---
         // rate1 is radians per frame (step)
         // Limit rate1:
         //   Mechanical, fast-slew: rate is limited to maximum mechanical rate
         //   Electronic, fast-slew: rate is unlimited!
         //   Mechanical, slow-slew: rate is commanded rate limited to max mechanical rate
         //   Electronic, slow-slew: rate is commanded rate (unlimited)
         // ---
         if (isFastSlewMode() && type == Type::MECHANICAL) {
               base::Vec3d step = maxRate * dt;
               limitVec(rate1, step);
         } else if (isSlowSlewMode()) {
               base::Vec3d cmdRate1 = cmdRate;
               if (type == Type::MECHANICAL) {
                  limitVec(cmdRate1, maxRate);
               }
               base::Vec3d step = cmdRate1 * dt;
               limitVec(rate1, step);
         }

         if (dt != 0.0) rate = rate1 * (1.0f/dt);
         else rate.set(0.0,0.0,0.0);
      }

      else if (servoMode == ServoMode::RATE) {
         // rate servo: follow commanded rate
         rate1 = cmdRate;

         // set servo rate to limited rate
         if (type == Type::MECHANICAL) limitVec(rate1, maxRate);

         rate = rate1;
      }

      // ---
      // Compute a new gimbal position
      //    newPos = pos{old} + dt*rate
      // ---
      base::Vec3d newPos{pos + (rate*dt)};
      newPos[AZ_IDX]   = base::angle::aepcdRad(newPos[AZ_IDX]);
      newPos[ELEV_IDX] = base::angle::aepcdRad(newPos[ELEV_IDX]);
      newPos[ROLL_IDX] = base::angle::aepcdRad(newPos[ROLL_IDX]);

      // ---
      // limit gimbal's azimuth position
      //  1) lowLimits[AZ_IDX] is the left limit;
      //     (unlimited if the limit is less than -180 deg)
      //  2) highLimits[AZ_IDX] is the right limit
      //     (unlimited if the limit is greater than 180 deg)
      // ---
      bool azLimit{};
      if (highLimits[AZ_IDX] >= -base::PI && highLimits[AZ_IDX] <= base::PI) {
         if ( base::angle::aepcdRad(highLimits[AZ_IDX] - newPos[AZ_IDX]) <= 0.0f ) {
            newPos[AZ_IDX] = highLimits[AZ_IDX];
            azLimit = true;
         }
      }
      if (lowLimits[AZ_IDX] >= -base::PI && lowLimits[AZ_IDX] <= base::PI) {
         if ( base::angle::aepcdRad(lowLimits[AZ_IDX] - newPos[AZ_IDX]) >= 0.0f) {
            newPos[AZ_IDX] = lowLimits[AZ_IDX];
            azLimit = true;
         }
      }

      // ---
      // limit gimbal's elevation position
      //  1) lowLimits[ELEV_IDX] is the lower limit;
      //     (unlimited if the limit is less than -180 deg)
      //  2) highLimits[ELEV_IDX] is the upper limit
      //     (unlimited if the limit is greater than 180 deg)
      // ---
      bool elLimit{};
      if (highLimits[ELEV_IDX] >= -base::PI && highLimits[ELEV_IDX] <= base::PI) {
         if ( base::angle::aepcdRad(highLimits[ELEV_IDX] - newPos[ELEV_IDX]) <= 0.0f ) {
            newPos[ELEV_IDX] = highLimits[ELEV_IDX];
            elLimit = true;
         }
      }
      if (lowLimits[ELEV_IDX] >= -base::PI && lowLimits[ELEV_IDX] <= base::PI) {
         if ( base::angle::aepcdRad(lowLimits[ELEV_IDX] - newPos[ELEV_IDX]) >= 0.0f) {
            newPos[ELEV_IDX] = lowLimits[ELEV_IDX];
            elLimit = true;
         }
      }

      // ---
      // limit gimbal's roll position
      //  1) lowLimits[XXXX] is the counter-clockwise limit;
      //     (unlimited if the limit is less than -180 deg)
      //  2) highLimits[XXXX] is the clockwise limit
      //     (unlimited if the limit is greater than 180 deg)
      // ---
      bool rollLimit{};
      if (highLimits[ROLL_IDX] >= -base::PI && highLimits[ROLL_IDX] <= base::PI) {
         if ( base::angle::aepcdRad(highLimits[ROLL_IDX] - newPos[ROLL_IDX]) <= 0.0f ) {
            newPos[ROLL_IDX] = highLimits[ROLL_IDX];
            rollLimit = true;
         }
      }
      if (lowLimits[ROLL_IDX] >= -base::PI && lowLimits[ROLL_IDX] <= base::PI) {
         if ( base::angle::aepcdRad(lowLimits[ROLL_IDX] - newPos[ROLL_IDX]) >= 0.0f) {
            newPos[ROLL_IDX] = lowLimits[ROLL_IDX];
            rollLimit = true;
         }
      }

      // update position
      atLimit = (azLimit || elLimit || rollLimit);
      pos = newPos;

   }

   // update matrix (a parent gimbal may have changed)
   updateMatrix();
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
double Gimbal::getAzimuthD() const
{
   return pos[AZ_IDX] * base::angle::R2DCC;
}

double Gimbal::getElevationD() const
{
   return pos[ELEV_IDX] * base::angle::R2DCC;
}

double Gimbal::getRollD() const
{
   return pos[ROLL_IDX] * base::angle::R2DCC;
}

void Gimbal::getAzimuthLimits(double* const leftLim, double* const rightLim) const
{
    if (leftLim != nullptr) *leftLim = lowLimits[AZ_IDX];
    if (rightLim != nullptr) *rightLim = highLimits[AZ_IDX];
}

void Gimbal::getElevationLimits(double* const lowerLim, double* const upperLim) const
{
    if (lowerLim != nullptr) *lowerLim = lowLimits[ELEV_IDX];
    if (upperLim != nullptr) *upperLim = highLimits[ELEV_IDX];
}

void Gimbal::getRollLimits(double* const lowerLim, double* const upperLim) const
{
    if (lowerLim != nullptr) *lowerLim = lowLimits[ROLL_IDX];
    if (upperLim != nullptr) *upperLim = highLimits[ROLL_IDX];
}

void Gimbal::getMaxRates(double* const azMaxRate, double* const ezMaxRate, double* const rollMaxRate) const
{
    if (azMaxRate != nullptr)   *azMaxRate = maxRate[AZ_IDX];
    if (ezMaxRate != nullptr)   *ezMaxRate = maxRate[ELEV_IDX];
    if (rollMaxRate != nullptr) *rollMaxRate = maxRate[ROLL_IDX];
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setType() -- sets our gimbal type
bool Gimbal::setType(const Type rt)
{
    type = rt;
    return true;
}

// setServoMode() -- Set servo mode: RATE_SERVO, POSITION_SERVO, etc.
bool Gimbal::setServoMode(const ServoMode m)
{
    servoMode = m;
    return true;
}

// setFastSlewMode() -- Set the fast slew flag
bool Gimbal::setFastSlewMode(const bool flg)
{
   fastSlew = flg;
   return true;
}

// Max range to players of interest or zero for all (meters)
bool Gimbal::setMaxRange2PlayersOfInterest(const double meters)
{
   maxRngPlayers = meters;
   return true;
}

// Max angle of gimbal boresight to players of interest or zero for all (rad)
bool Gimbal::setMaxAngle2PlayersOfInterest(const double radians)
{
   maxAnglePlayers = radians;
   return true;
}

// Player of interest types (Player::MajorType bit-wise or'd)
bool Gimbal::setPlayerOfInterestTypes(const unsigned int typeMask)
{
   playerTypes = typeMask;
   return true;
}

// Max number of players of interest (i.e., size of the arrays)
bool Gimbal::setMaxPlayersOfInterest(const unsigned int n)
{
   maxPlayers = n;
   return true;
}

// Sets the local only players of interest flag
bool Gimbal::setLocalPlayersOfInterestOnly(const bool flg)
{
   localOnly = flg;
   return true;
}

// Sets the target terrain occulting enabled flag
bool Gimbal::setTerrainOccultingEnabled(const bool flg)
{
   terrainOcculting = flg;
   return true;
}

// Sets the horizon check enabled flag
bool Gimbal::setHorizonCheckEnabled(const bool flg)
{
   checkHorizon = flg;
   return true;
}

// Sets the using world coordinates flag
bool Gimbal::setUseWorld(const bool flg)
{
   useWorld = flg;
   return true;
}

// Sets the own heading only flag
bool Gimbal::setOwnHeadingOnly(const bool flg)
{
   ownHeadingOnly = flg;
   return true;
}


//------------------------------------------------------------------------------
// setPosition() - sets the initial azimuth and elevation position
//------------------------------------------------------------------------------
bool Gimbal::setPosition(const double azim, const double elev, const double roll)
{
    pos.set(azim, elev, roll);
    return true;
}

bool Gimbal::setPosition(const double azim, const double elev)
{
    return setPosition(azim, elev, 0);
}

//------------------------------------------------------------------------------
// setMaxRates:  set az & el rate limits
//------------------------------------------------------------------------------
bool Gimbal::setMaxRates(const double azMaxRate, const double elMaxRate, const double rollMaxRate)
{
    maxRate.set( std::fabs(azMaxRate), std::fabs(elMaxRate), std::fabs(rollMaxRate) );
    return true;
}

bool Gimbal::setMaxRates(const double azMaxRate, const double elMaxRate)
{
    return setMaxRates( std::fabs(azMaxRate), std::fabs(elMaxRate), 0 );
}

//------------------------------------------------------------------------------
// setAzimuthLimits() - sets our lower and upper azimuth limits
//------------------------------------------------------------------------------
bool Gimbal::setAzimuthLimits(const double leftLim, const double rightLim)
{
    lowLimits[AZ_IDX]  = leftLim;
    highLimits[AZ_IDX] = rightLim;
    return true;
}

//------------------------------------------------------------------------------
// setElevationLimits:  set lower & upper elevation limits
//------------------------------------------------------------------------------
bool Gimbal::setElevationLimits(const double lowerLim, const double upperLim)
{
    lowLimits[ELEV_IDX]  = lowerLim;
    highLimits[ELEV_IDX] = upperLim;
    return true;
}

//------------------------------------------------------------------------------
// setRollLimits:  set lower & upper roll limits
//------------------------------------------------------------------------------
bool Gimbal::setRollLimits(const double lowerLim, const double upperLim)
{
    lowLimits[ROLL_IDX]  = lowerLim;
    highLimits[ROLL_IDX] = upperLim;
    return true;
}

//------------------------------------------------------------------------------
// setCmdRate() -- put the gimbal servo in rate mode and set the commanded rate.
//------------------------------------------------------------------------------
bool Gimbal::setCmdRate(const base::Vec3d& r)
{
   cmdRate = r;
   setServoMode(ServoMode::RATE);
   return true;
}

bool Gimbal::setCmdRate(const base::Vec2d& r)
{
   base::Vec3d t;
   t[AZ_IDX]   = r[AZ_IDX];
   t[ELEV_IDX] = r[ELEV_IDX];
   t[ROLL_IDX] = 0;  // default roll to zero
   return setCmdRate(t);
}

//------------------------------------------------------------------------------
// setCmdRate() -- put the gimbal servo in rate mode and set the commanded rate.
//------------------------------------------------------------------------------
bool Gimbal::setCmdRate(const double azRate, const double elRate, const double rollRate)
{
   cmdRate.set(azRate,elRate,rollRate);
   setServoMode(ServoMode::RATE);
   return true;
}

bool Gimbal::setCmdRate(const double azRate, const double elRate)
{
   return setCmdRate(azRate, elRate, 0);
}

//------------------------------------------------------------------------------
// setCmdPos() -- put the gimbal servo in position mode and set the commanded position.
//------------------------------------------------------------------------------
bool Gimbal::setCmdPos(const base::Vec3d& p)
{
   // ---
   // Limit range from -pi to pi
   // ---
   base::Vec3d newPos{p};
   newPos[AZ_IDX]   = base::angle::aepcdRad(newPos[AZ_IDX]);
   newPos[ELEV_IDX] = base::angle::aepcdRad(newPos[ELEV_IDX]);
   newPos[ROLL_IDX] = base::angle::aepcdRad(newPos[ROLL_IDX]);

   // ---
   // limit commanded azimuth position
   //  1) lowLimits[AZ_IDX] is the left limit;
   //     (unlimited if the limit is less than -180 deg)
   //  2) highLimits[AZ_IDX] is the right limit
   //     (unlimited if the limit is greater than 180 deg)
   // ---
   if (highLimits[AZ_IDX] >= -base::PI && highLimits[AZ_IDX] <= base::PI) {
      if ( base::angle::aepcdRad(highLimits[AZ_IDX] - newPos[AZ_IDX]) <= 0.0f ) {
         newPos[AZ_IDX] = highLimits[AZ_IDX];
      }
   }
   if (lowLimits[AZ_IDX] >= -base::PI && lowLimits[AZ_IDX] <= base::PI) {
      if ( base::angle::aepcdRad(lowLimits[AZ_IDX] - newPos[AZ_IDX]) >= 0.0f) {
         newPos[AZ_IDX] = lowLimits[AZ_IDX];
      }
   }

   // ---
   // limit commanded elevation position
   //  1) lowLimits[ELEV_IDX] is the lower limit;
   //     (unlimited if the limit is less than -180 deg)
   //  2) highLimits[ELEV_IDX] is the upper limit
   //     (unlimited if the limit is greater than 180 deg)
   // ---
   if (highLimits[ELEV_IDX] >= -base::PI && highLimits[ELEV_IDX] <= base::PI) {
      if ( base::angle::aepcdRad(highLimits[ELEV_IDX] - newPos[ELEV_IDX]) <= 0.0f ) {
         newPos[ELEV_IDX] = highLimits[ELEV_IDX];
      }
   }
   if (lowLimits[ELEV_IDX] >= -base::PI && lowLimits[ELEV_IDX] <= base::PI) {
      if ( base::angle::aepcdRad(lowLimits[ELEV_IDX] - newPos[ELEV_IDX]) >= 0.0f) {
         newPos[ELEV_IDX] = lowLimits[ELEV_IDX];
      }
   }

   // ---
   // limit commanded roll position
   //  1) lowLimits[ROLL_IDX] is the counter-clockwise limit;
   //     (unlimited if the limit is less than -180 deg)
   //  2) highLimits[ROLL_IDX] is the clockwise limit
   //     (unlimited if the limit is greater than 180 deg)
   // ---
   if (highLimits[ROLL_IDX] >= -base::PI && highLimits[ROLL_IDX] <= base::PI) {
      if ( base::angle::aepcdRad(highLimits[ROLL_IDX] - newPos[ROLL_IDX]) <= 0.0f ) {
         newPos[ROLL_IDX] = highLimits[ROLL_IDX];
      }
   }
   if (lowLimits[ROLL_IDX] >= -base::PI && lowLimits[ROLL_IDX] <= base::PI) {
      if ( base::angle::aepcdRad(lowLimits[ROLL_IDX] - newPos[ROLL_IDX]) >= 0.0f) {
         newPos[ROLL_IDX] = lowLimits[ROLL_IDX];
      }
   }

   // ---
   // Set the new (limited) commanded position
   // ---
   cmdPos = newPos;
   setServoMode(ServoMode::POSITION);

   return true;
}

bool Gimbal::setCmdPos(const base::Vec2d& p)
{
   base::Vec3d t;
   t[AZ_IDX]   = p[AZ_IDX];
   t[ELEV_IDX] = p[ELEV_IDX];
   t[ROLL_IDX] = 0;  // default roll to zero
   return setCmdPos(t);
}

bool Gimbal::setCmdPos(const double az, const double el, const double roll)
{
   base::Vec3d t;
   t[AZ_IDX]   = az;
   t[ELEV_IDX] = el;
   t[ROLL_IDX] = roll;
   return setCmdPos(t);
}

//------------------------------------------------------------------------------
// setLocation() -- set the location of the gimbal
//------------------------------------------------------------------------------
bool Gimbal::setLocation(const double x, const double y, const double z)
{
    location.set(x, y, z);
    return true;
}

//------------------------------------------------------------------------------
// Slot functions ---
//------------------------------------------------------------------------------

bool Gimbal::setSlotType(const base::Identifier* const x)
{
    if (x == nullptr) return false;

    bool ok{true};
    if (*x == "mechanical") ok = setType(Type::MECHANICAL);
    else if (*x == "electronic") ok = setType(Type::ELECTRONIC);
    else ok = false;
    return ok;
}

// setSlotLocation() -
bool Gimbal::setSlotLocation(const base::List* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      double values[3]{};
      const std::size_t n{msg->getNumberList(values, 3)};
      if (n == 3) ok = setLocation(values[0], values[1], values[2]);
   }
   return ok;
}

// setSlotPosition() - Initial positon vector (radians) [ az el roll ]
bool Gimbal::setSlotPosition(const base::List* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      double values[3]{};
      const std::size_t n{msg->getNumberList(values, 3)};
      if (n == 2) ok = setPosition(values[0], values[1]);
      else if (n == 3) ok = setPosition(values[0], values[1], values[2]);
      if (ok) initPos = pos;
   }
    return ok;
}

// setSlotPosAzimuth() - Initial azimuth positon
bool Gimbal::setSlotPosAzimuth(const base::Angle* const az)
{
   bool ok{};
   if (az != nullptr) {
       ok = setPosition(az->getValueInRadians(), getElevation(), getRoll());
       if (ok) initPos = pos;
   }
   return ok;
}

// setSlotPosElevation() - Initial elevation positon
bool Gimbal::setSlotPosElevation(const base::Angle* const el)
{
   bool ok{};
   if (el != nullptr) {
       ok = setPosition(getAzimuth(), el->getValueInRadians(), getRoll());
       if (ok) initPos = pos;
   }
   return ok;
}

// setSlotPosRoll() - Initial roll positon
bool Gimbal::setSlotPosRoll(const base::Angle* const roll)
{
   bool ok{};
   if (roll != nullptr) {
       ok = setPosition(getAzimuth(), getElevation(), roll->getValueInRadians());
       if (ok) initPos = pos;
   }
   return ok;
}

// setSlotAzimuthLimits() - Azimuth limit vector (radians) [ left right ]
bool Gimbal::setSlotAzimuthLimits(const base::List* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double values[2]{};
        const std::size_t n{msg->getNumberList(values, 2)};
        if (n == 2) ok = setAzimuthLimits(values[0], values[1]);  // (left, right)
    }
    return ok;
}

// setSlotAzimuthLimitLeft() - Left azimuth limit
bool Gimbal::setSlotAzimuthLimitLeft(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double leftLim{}, rightLim{};
        getAzimuthLimits(&leftLim, &rightLim);
        ok = setAzimuthLimits(msg->getValueInRadians(), rightLim);
    }
    return ok;
}

// setSlotAzimuthLimitRight() - Right azimuth limit
bool Gimbal::setSlotAzimuthLimitRight(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double leftLim{}, rightLim{};
        getAzimuthLimits(&leftLim, &rightLim);
        ok = setAzimuthLimits(leftLim, msg->getValueInRadians());
    }
    return ok;
}

// setSlotElevationLimits() - calls setElevationLimits()
bool Gimbal::setSlotElevationLimits(const base::List* const numList)
{
    bool ok{};
    if (numList != nullptr) {
        double values[2]{};
        const std::size_t n{numList->getNumberList(values, 2)};
        if (n == 2) ok = setElevationLimits(values[0], values[1]);
    }
    return ok;
}

// setSlotElevationLower() - Lower elevation limit
bool Gimbal::setSlotElevationLower(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double lowerLim{}, upperLim{};
        getElevationLimits(&lowerLim, &upperLim);
        ok = setElevationLimits(msg->getValueInRadians(), upperLim);
    }
    return ok;
}

// setSlotElevationUpper() - Upper elevation limit
bool Gimbal::setSlotElevationUpper(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double lowerLim{}, upperLim{};
        getElevationLimits(&lowerLim, &upperLim);
        ok = setElevationLimits(lowerLim, msg->getValueInRadians());
    }
    return ok;
}


// setSlotRollLimits() - calls setRollLimits()
bool Gimbal::setSlotRollLimits(const base::List* const numList)
{
    bool ok{};
    if (numList != nullptr) {
        double values[2]{};
        const std::size_t n{numList->getNumberList(values, 2)};
        if (n == 2) ok = setRollLimits(values[0], values[1]);
    }
    return ok;
}

// setSlotRollLimitLower() - Lower roll limit
bool Gimbal::setSlotRollLimitLower(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double lowerLim{}, upperLim{};
        getRollLimits(&lowerLim, &upperLim);
        ok = setRollLimits(msg->getValueInRadians(), upperLim);
    }
    return ok;
}

// setSlotRollLimitUpper() - Upper roll limit
bool Gimbal::setSlotRollLimitUpper(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double lowerLim{}, upperLim{};
        getRollLimits(&lowerLim, &upperLim);
        ok = setRollLimits(lowerLim, msg->getValueInRadians());
    }
    return ok;
}

// setSlotMaxRates() -- calls setMaxRates()
bool Gimbal::setSlotMaxRates(const base::List* const numList)
{
    bool ok{};
    if (numList != nullptr) {
        double values[3]{};
        const std::size_t n{numList->getNumberList(values, 3)};
        if (n == 2) ok = setMaxRates(values[0], values[1]);
        else if (n == 3) ok = setMaxRates(values[0], values[1], values[2]);
    }

    return ok;
}

// setSlotMaxRateAzimuth() - Max "mechanical" azimuth rate (base::Angle/sec)
bool Gimbal::setSlotMaxRateAzimuth(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double azMaxRate{}, ezMaxRate{}, rollMaxRate{};
        getMaxRates(&azMaxRate, &ezMaxRate, &rollMaxRate);
        ok = setMaxRates(msg->getValueInRadians(), ezMaxRate, rollMaxRate);
    }
    return ok;
}

// setSlotMaxRateElevation() - Max "mechanical" elevation rate (base::Angle/sec)
bool Gimbal::setSlotMaxRateElevation(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double azMaxRate{}, ezMaxRate{}, rollMaxRate{};
        getMaxRates(&azMaxRate, &ezMaxRate, &rollMaxRate);
        ok = setMaxRates(azMaxRate, msg->getValueInRadians(), rollMaxRate);
    }
    return ok;
}

// setSlotMaxRateRoll() - Max "mechanical" roll rate (base::Angle/sec)
bool Gimbal::setSlotMaxRateRoll(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double azMaxRate{}, ezMaxRate{}, rollMaxRate{};
        getMaxRates(&azMaxRate, &ezMaxRate, &rollMaxRate);
        ok = setMaxRates(azMaxRate, ezMaxRate, msg->getValueInRadians());
    }
    return ok;
}

// setSlotCmdPos() -- calls setCmdPos()
bool Gimbal::setSlotCmdPos(const base::List* const numList)
{
    bool ok{};
    if (numList != nullptr) {
      double values[3]{};
      const std::size_t n{numList->getNumberList(values, 3)};
      if (n == 2) ok = setCmdPos(values[0],values[1]);
      else if (n == 3) ok = setCmdPos(values[0], values[1], values[2]);
      if (ok) initCmdPos = cmdPos;
    }

    return ok;
}

// setSlotCmdPosAzimuth - Commanded azimuth position  (sets POSITION_SERVO)
bool Gimbal::setSlotCmdPosAzimuth(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setCmdPos(msg->getValueInRadians(), getCmdElev(), getCmdRoll());
        if (ok) initCmdPos = cmdPos;
    }
    return ok;
}

// setSlotCmdPosElevation() - Commanded elevation position (sets POSITION_SERVO)
bool Gimbal::setSlotCmdPosElevation(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setCmdPos(getCmdAz(), msg->getValueInRadians(), getCmdRoll());
        if (ok) initCmdPos = cmdPos;
    }
    return ok;
}

// setSlotCmdPosRoll() - Commanded roll position  (sets POSITION_SERVO)
bool Gimbal::setSlotCmdPosRoll(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setCmdPos(getCmdAz(), getCmdElev(), msg->getValueInRadians());
        if (ok) initCmdPos = cmdPos;
    }
    return ok;
}

// setSlotCmdRate() -- calls setCmdRate()
bool Gimbal::setSlotCmdRate(const base::List* const numList)
{
   bool ok{};
   if (numList != nullptr) {
      double values[3]{};
      const std::size_t n{numList->getNumberList(values, 3)};
      if (n == 2) ok = setCmdRate(values[0],values[1]);
      else if (n == 3) ok = setCmdRate(values[0],values[1],values[2]);
      if (ok) initCmdRate = cmdRate;
   }
   return ok;
}

// setSlotCmdRateAzimuth() - Commanded azimuth rate (sets RATE_SERVO)
bool Gimbal::setSlotCmdRateAzimuth(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setCmdRate(msg->getValueInRadians(), getCmdElevRate(), getCmdRollRate());
        if (ok) initCmdRate = cmdRate;
    }
    return ok;
}

// setSlotCmdRateElevation() - Commanded elevation rate (sets RATE_SERVO)
bool Gimbal::setSlotCmdRateElevation(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setCmdRate(getCmdAzRate(), msg->getValueInRadians(), getCmdRollRate());
        if (ok) initCmdRate = cmdRate;
    }
    return ok;
}

// setSlotCmdRateRoll() - Commanded roll rate (sets RATE_SERVO)
bool Gimbal::setSlotCmdRateRoll(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setCmdRate(getCmdAzRate(), getCmdElevRate(), msg->getValueInRadians());
        if (ok) initCmdRate = cmdRate;
    }
    return ok;
}

// Enable target terrain occulting (default: false)
bool Gimbal::setSlotTerrainOcculting(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setTerrainOccultingEnabled(msg->asBool());
   }
   return ok;
}

// Enable horizon masking check (default: true)
bool Gimbal::setSlotCheckHorizon(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setHorizonCheckEnabled(msg->asBool());
   }
   return ok;
}

// Player of interest types (default: 0 )
bool Gimbal::setSlotPlayerTypes(const base::PairStream* const x)
{
   bool ok{};
   if (x != nullptr) {
      unsigned int mask{};
      const base::List::Item* item{x->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<const base::Pair*>(item->getValue());
         const auto type = dynamic_cast<const base::Identifier*>( pair->object() );
         if (type != nullptr) {
            if ( *type == "air") {
               mask = (mask | Player::AIR_VEHICLE);
            } else if ( *type == "ground" ) {
               mask = (mask | Player::GROUND_VEHICLE);
            } else if ( *type == "weapon" ) {
               mask = (mask | Player::WEAPON);
            } else if ( *type == "ship" ) {
               mask = (mask | Player::SHIP);
            } else if ( *type == "building" ) {
               mask = (mask | Player::BUILDING);
            }  else if ( *type == "lifeform" ) {
               mask = (mask | Player::LIFE_FORM);
            }  else if ( *type == "space" ) {
               mask = (mask | Player::SPACE_VEHICLE);
            }
         }
         item = item->getNext();
      }
      ok = setPlayerOfInterestTypes(mask);
   }
   return ok;
}

// Max number of players of interest (default: 0)
bool Gimbal::setSlotMaxPlayers(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setMaxPlayersOfInterest( msg->asInt() );
   }
   return ok;
}

// Max range to players of interest or zero for all (meters)
bool Gimbal::setSlotMaxRange2PlayersOfInterest(const base::Length* const x)
{
    bool ok{};
    if (x != nullptr) {
        ok = setMaxRange2PlayersOfInterest(x->getValueInMeters());
    }
    return ok;
}

// Max angle of gimbal boresight to players of interest or zero for all (rad)
bool Gimbal::setSlotMaxAngle2PlayersOfInterest(const base::Angle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setMaxAngle2PlayersOfInterest(msg->getValueInRadians());
    }
    return ok;
}

// Sets the local only players of interest flag
bool Gimbal::setSlotLocalPlayersOfInterestOnly(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setLocalPlayersOfInterestOnly(msg->asBool());
   }
   return ok;
}

// Using player of interest's world (ECEF) coordinate system
bool Gimbal::setSlotUseWorldCoordinates(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setUseWorld(msg->asBool());
   }
   return ok;
}

// Sets the own heading only flag
bool Gimbal::setSlotUseOwnHeadingOnly(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setOwnHeadingOnly(msg->asBool());
   }
   return ok;
}

//------------------------------------------------------------------------------
// updateMatrix() -- update the A/C coord to gimbal's coord matrix
//------------------------------------------------------------------------------
void Gimbal::updateMatrix()
{
   // Start with a rotational matrix
   base::Matrixd mm1;
   base::nav::computeRotationalMatrix( getRoll(), getElevation(), getAzimuth(), &mm1);

   // Post multiply by a translate to our location
   base::Matrixd rr;
   rr.makeTranslate(getLocation());
   mm1 *= rr;

   // ---
   // Is our parent (container) also a gimbal?
   // If so then post multiply by our container's matrix
   // ---
   const auto p = dynamic_cast<const Gimbal*>( container() );
   if (p != nullptr) { mm1 *= p->getRotMat(); }

   // Set as the new matrix
   tm = mm1;
}

//------------------------------------------------------------------------------
// isPositioned() -- returns true of the gimbal has reached the commanded pos
//------------------------------------------------------------------------------
bool Gimbal::isPositioned(const double tol0) const
{
   double tol{tol0};
   if (tol < 0) tol = defaultTolerance;

   base::Vec3d t;
   t[AZ_IDX]   = base::angle::aepcdRad( pos[AZ_IDX]   - cmdPos[AZ_IDX] );
   t[ELEV_IDX] = base::angle::aepcdRad( pos[ELEV_IDX] - cmdPos[ELEV_IDX] );
   t[ROLL_IDX] = base::angle::aepcdRad( pos[ROLL_IDX] - cmdPos[ROLL_IDX] );
   return t.length2() < (tol*tol);
}

//------------------------------------------------------------------------------
// isAtLimits() -- Returns true if the gimbal is at a 'physical' limit
//------------------------------------------------------------------------------
bool Gimbal::isAtLimits() const
{
   return atLimit;
}

//------------------------------------------------------------------------------
// limitVec(Vec,lim) -- symmetrical limit of a vector
//------------------------------------------------------------------------------
void Gimbal::limitVec(base::Vec2d& vec, const base::Vec2d& lim)
{
  double l0{std::fabs(lim[0])};
  if (vec[0] >  l0)  { vec[0] =  l0; }
  if (vec[0] < -l0)  { vec[0] = -l0; }

  double l1{std::fabs(lim[1])};
  if (vec[1] >  l1)  { vec[1] =  l1; }
  if (vec[1] < -l1)  { vec[1] = -l1; }
}

void Gimbal::limitVec(base::Vec3d& vec, const base::Vec3d& lim)
{
  double l0{std::fabs(lim[0])};
  if (vec[0] >  l0)  { vec[0] =  l0; }
  if (vec[0] < -l0)  { vec[0] = -l0; }

  double l1{std::fabs(lim[1])};
  if (vec[1] >  l1)  { vec[1] =  l1; }
  if (vec[1] < -l1)  { vec[1] = -l1; }

  double l2{std::fabs(lim[2])};
  if (vec[2] >  l2)  { vec[2] =  l2; }
  if (vec[2] < -l2)  { vec[2] = -l2; }
}

//------------------------------------------------------------------------------
// limitVec(vec,lower,upper) -- asymmetrical limit of a vector
//------------------------------------------------------------------------------
void Gimbal::limitVec(base::Vec2d& vec, const base::Vec2d& ll, const base::Vec2d& ul)
{
  if (vec[0] > ul[0])  { vec[0] = ul[0]; }
  if (vec[0] < ll[0])  { vec[0] = ll[0]; }
  if (vec[1] > ul[1])  { vec[1] = ul[1]; }
  if (vec[1] < ll[1])  { vec[1] = ll[1]; }
}

void Gimbal::limitVec(base::Vec3d& vec, const base::Vec3d& ll, const base::Vec3d& ul)
{
  if (vec[0] > ul[0])  { vec[0] = ul[0]; }
  if (vec[0] < ll[0])  { vec[0] = ll[0]; }
  if (vec[1] > ul[1])  { vec[1] = ul[1]; }
  if (vec[1] < ll[1])  { vec[1] = ll[1]; }
  if (vec[2] > ul[2])  { vec[2] = ul[2]; }
  if (vec[2] < ll[2])  { vec[2] = ll[2]; }
}

//------------------------------------------------------------------------------
// Process the Players-Of-Interest (POI) list
//------------------------------------------------------------------------------
unsigned int Gimbal::processPlayersOfInterest(base::PairStream* const poi)
{
   const auto tdb0 = new Tdb(maxPlayers, this);

   unsigned int ntgts{tdb0->processPlayers(poi)};
   setCurrentTdb(tdb0);
   tdb0->unref();

   return ntgts;
}

//------------------------------------------------------------------------------
// Returns the current TDB (pre-ref())
//------------------------------------------------------------------------------
Tdb* Gimbal::getCurrentTDB()
{
   return tdb.getRefPtr();
}

//------------------------------------------------------------------------------
// Returns the current TDB (pre-ref())
//------------------------------------------------------------------------------
const Tdb* Gimbal::getCurrentTDB() const
{
   return tdb.getRefPtr();
}

//------------------------------------------------------------------------------
// Set the current TDB
//------------------------------------------------------------------------------
bool Gimbal::setCurrentTdb(Tdb* const newTdb)
{
   tdb = newTdb;
   return true;
}

}
}
