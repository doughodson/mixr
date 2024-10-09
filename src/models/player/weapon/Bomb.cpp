
#include "mixr/models/player/weapon/Bomb.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Number.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Bomb, "Bomb")
EMPTY_DELETEDATA(Bomb)

BEGIN_SLOTTABLE(Bomb)
    "arming",           // 1: Bomb arming option
    "noseFuze",         // 2: Nose fuze flag
    "midFuze",          // 3: Middle fuze flag
    "tailFuze",         // 4: Tail fuze flag
    "fuzeAltitude",     // 5: Fuze arming Altitude
    "fuzeTime",         // 6: Fuze Arming Time
    "dragIndex",        // 7: drag index used by default dynamics
END_SLOTTABLE(Bomb)

BEGIN_SLOT_MAP(Bomb)
   ON_SLOT( 1, setSlotArmingOption, base::Identifier)
   ON_SLOT( 2, setSlotNoseFuze,     base::Boolean)
   ON_SLOT( 3, setSlotMidFuze,      base::Boolean)
   ON_SLOT( 4, setSlotTailFuze,     base::Boolean)
   ON_SLOT( 5, setSlotFuzeAltitude, base::Number)
   ON_SLOT( 6, setSlotFuzeTime,     base::Number)
   ON_SLOT( 7, setSlotDragIndex,    base::Number)
END_SLOT_MAP()

// Weapon data for general bomb
int Bomb::getCategory() const                { return GRAVITY; }
const char* Bomb::getDescription() const     { return "Generic Bomb"; }
const char* Bomb::getNickname() const        { return "Bomb"; }

Bomb::Bomb()
{
   STANDARD_CONSTRUCTOR()

   static base::String generic("Bomb");
   setType_old(&generic);
   setType("Bomb");

   initData();
}

void Bomb::initData()
{
   missDistRef.set(0,0,0);
   tgtRangeRef.set(0,0,0);

   setMaxTOF(60.0);
   setLethalRange(50.0);
   setMaxBurstRng(500.0);
   setTSG(9999.0);
   setSOBT(9999.0);
   setEOBT(0.0);
}

void Bomb::copyData(const Bomb& org, const bool cc)
{
    BaseClass::copyData(org);
   if (cc) initData();

    arming    = org.arming;
    noseFuze  = org.noseFuze;
    midFuze   = org.midFuze;
    tailFuze  = org.tailFuze;
    fuzeAlt   = org.fuzeAlt;
    fuzeTime  = org.fuzeTime;
    dragIndex     = org.dragIndex;
    missDistRef   = org.missDistRef;;
    tgtRangeRef   = org.tgtRangeRef;
    cmdStrAz      = org.cmdStrAz;
    cmdStrEl      = org.cmdStrEl;
    guidanceValid = org.guidanceValid;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------
unsigned int Bomb::getArmingOption() const
{
   return arming;
}

bool Bomb::isArmingOption(const unsigned int a) const
{
   return (a == arming);
}

double Bomb::getFuzeAltitude() const
{
   return fuzeAlt;
}

double Bomb::getFuzeTime() const
{
   return fuzeTime;
}

double Bomb::getDragIndex() const
{
   return dragIndex;
}

bool Bomb::isNoseFuze() const
{
   return noseFuze;
}

bool Bomb::isMidFuze() const
{
   return midFuze;
}

bool Bomb::isTailFuze() const
{
   return tailFuze;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------
bool Bomb::setNoseFuze(const bool f)
{
   noseFuze = f;
   return true;
}

bool Bomb::setMidFuze(const bool f)
{
   midFuze = f;
   return true;
}

bool Bomb::setTailFuze(const bool f)
{
   tailFuze = f;
   return true;
}

bool Bomb::setArmingOption(const unsigned int a)
{
   arming = a;
   return true;
}

bool Bomb::setFuzeAltitude(const double v)
{
   fuzeAlt = v;
   return true;
}

bool Bomb::setFuzeTime(const double v)
{
   fuzeTime = v;
   return true;
}

bool Bomb::setDragIndex(const double v)
{
   dragIndex = v;
   return true;
}

//------------------------------------------------------------------------------
// weaponGuidance() -- default guidance; using Robot Aircraft (RAC) guidance
//------------------------------------------------------------------------------
void Bomb::weaponGuidance(const double)
{
   if (isGuidanceEnabled()) {

      // Get target position
      const base::Vec3d tgtPos{getTargetPosition()};

      // weapon velocities (platform)
      const base::Vec3d vel{getVelocity()};

      // weapon velocities (relative)
      double velG{std::sqrt (vel[0]*vel[0] + vel[1]*vel[1])};
      if (velG < 1.0f) velG = 1.0f;

      // Earth to platform sin/cos
      const double prcos{vel[0] / velG};
      const double prsin{vel[1] / velG};

      // Target position (platform)
      const base::Vec3d tgtPosP{tgtPos - getPosition()};

      // Target range distance
      const double tsrng{tgtPosP.length()};

      // Target position (relative)
      tgtRangeRef.set(
            (  prcos * tgtPosP[0] + prsin * tgtPosP[1] ),
            ( -prsin * tgtPosP[0] + prcos * tgtPosP[1] ),
            ( tgtPosP[2] )
         );

      // ---
      // Estimate impact point
      // ---

      // Inputs
      base::Vec3d initPos(0.0, 0.0, static_cast<double>(-getAltitude()));
      base::Vec3d initVel{vel};
      double groundPlane{tgtPos[2]};
      double timeStep{0.1};
      double maxTime{60.0};

      // Outputs
      double tof{};
      base::Vec3d ip;

      // Predict platform impact point
      weaponImpactPrediction(&initPos, &initVel, groundPlane, timeStep, maxTime, dragIndex, &ip, &tof);
      ip[2] = tgtPosP[2];

      // ---
      // Compute miss distance
      // ---

      // miss Dist (Platform)
      const base::Vec3d pmd{tgtPosP - ip};

      // rotate to ref coord sys
      missDistRef.set(
            (  prcos * pmd[0] + prsin * pmd[1] ),  // X MISS DIST (REF)
            ( -prsin * pmd[0] + prcos * pmd[1] ),  // Y MISS DIST (REF)
            ( pmd[2] )
         );

      // Azimuth & elevation steering
      static const double LIMITS{1.0};
      static const double GAIN{5.0};
      double caz{std::atan2( missDistRef.y(), tsrng ) * GAIN};
      if (caz >  LIMITS) caz =  LIMITS;
      if (caz < -LIMITS) caz = -LIMITS;
      cmdStrAz = caz;

      double cel{std::atan2( missDistRef.x(), tsrng ) * GAIN};
      if (cel >  LIMITS) cel =  LIMITS;
      if (cel < -LIMITS) cel = -LIMITS;
      cmdStrEl = cel;

      guidanceValid = true;

   } else {
      missDistRef.set(0,0,0);
      tgtRangeRef.set(0,0,0);
      cmdStrAz = 0;
      cmdStrEl = 0;
      guidanceValid = false;
   }
}


//------------------------------------------------------------------------------
// weaponDynamics -- default dynamics; using Robot Aircraft (RAC) dynamics
//------------------------------------------------------------------------------
void Bomb::weaponDynamics(const double dt)
{
   // Useful constant
   static const double g{base::ETHG * base::length::FT2M};      // Acceleration of Gravity (m/s/s)

   // ---
   // Compute & Set acceleration vector (earth)
   // ---

   // First drag
   const base::Vec3d tmp{getVelocity() * (-dragIndex)};

   // then gravity
   base::Vec3d ae1{tmp};
   ae1[IDOWN] += g;

   // and the controls from guidance, if any
   if (guidanceValid) {
      const double h{g * 2.0};
      base::Vec3d ab0(0, 0, 0); // body accelerations

      ab0[1] = h * cmdStrAz;  // body Y accel (m/s/s)
      if (ab0[1] >  h) ab0[1] = h;
      if (ab0[1] < -h) ab0[1] = -h;

      ab0[2] = -g * cmdStrEl;  // body Z accel (m/s/s)
      if (ab0[2] >  (g/5.0f)) ab0[2] = (g/5.0f);
      if (ab0[2] < -h) ab0[2] = -h;

      // rotate to earth and add to accel vector
      base::Vec3d ae0{ab0 * getRotMat()};
      ae1 = ae1 + ae0;
   }

   // ---
   // Compute new velocity vector (earth)
   // ---
   const base::Vec3d ve1{getVelocity() + (ae1 * dt)};

   // ---
   // Compute Euler angles
   // ---
   const double vg{std::sqrt(ve1[INORTH]*ve1[INORTH] + ve1[IEAST]*ve1[IEAST])};
   const double newPsi{std::atan2(ve1[IEAST],ve1[INORTH])};
   const double newTheta{std::atan2( -ve1[IDOWN], vg )};


   // ---
   // Set the new values: Euler angles first then velocities
   // ---
   setEulerAngles(0.0, newTheta, newPsi);
   setVelocity(ve1);
   setAcceleration(ae1);
   setAngularVelocities(0.0, 0.0, 0.0);
}

//------------------------------------------------------------------------------
// weaponPrediction -- Predict weapon impact point
//------------------------------------------------------------------------------
bool Bomb::impactPrediction(
      const base::Vec3d* const initPos,
      const base::Vec3d* const initVel,
      const double groundPlane,
      const double dt,
      const double maxTime,
      base::Vec3d* const finalPos,
      double* const tof
   ) const
{
   return weaponImpactPrediction(initPos, initVel, groundPlane, dt, maxTime, dragIndex, finalPos, tof);
}

//------------------------------------------------------------------------------
// Predict weapon impact point
//------------------------------------------------------------------------------
bool Bomb::weaponImpactPrediction(
      const base::Vec3d* const initPos,
      const base::Vec3d* const initVel,
      const double groundPlane,
      const double dt,
      const double maxTime,
      const double dragIndex,
      base::Vec3d* const finalPos,
      double* const tof
   )
{
   // Useful constant
   static const double g{base::ETHG * base::length::FT2M};      // Acceleration of Gravity (m/s/s)

   // Make sure we have all of our pointers
   if (initPos == nullptr || initVel == nullptr || finalPos == nullptr || tof == nullptr) return false;

   // Set initial position & velocity
   base::Vec3d pos{*initPos};
   base::Vec3d vel{*initVel};

   // Run time
   double time{};

   // ---
   // Simple ballistic extrapolate of the weapon's position
   // ---

   while (time < maxTime && (pos[IDOWN] < groundPlane)) {

      // Compute acceleration vector (earth) ...

      //    first drag,
      base::Vec3d ae1 = vel * (-dragIndex);

      //   then gravity
      ae1[IDOWN] += g;

      // Compute new velocity vector (earth)
      vel = vel + (ae1 * dt);

      // Compute new position vector (earth)
      pos = pos + (vel * dt);

      time += dt;
   }

   // Set the results back to the caller
   *finalPos = pos;
   *tof = time;
   return true;
}

//------------------------------------------------------------------------------
// Set Slot routines --
//------------------------------------------------------------------------------

bool Bomb::setSlotArmingOption(base::Identifier* const p)
{
    bool ok{};
    if (p != nullptr) {
       if (*p == "NONE" || *p == "none") ok = setArmingOption(NONE);
       else if (*p == "FREE_FALL" || *p == "free_fall") ok = setArmingOption(FREE_FALL);
       else if (*p == "RETARDED" || *p == "retarded") ok = setArmingOption(RETARDED);
       else if (*p == "TIMER" || *p == "timer") ok = setArmingOption(TIMER);
       else if (*p == "PROXIMITY" || *p == "proximity") ok = setArmingOption(PROXIMITY);
       if (!ok) {
          std::cerr << "Bomb::setSlotArmingOption() Invalid arming option: " << *p << std::endl;
    }
    }
    return ok;
}

// noseFuze: Nose fuze flag
bool Bomb::setSlotNoseFuze(base::Boolean* const p)
{
    setNoseFuze( p->asBool() );
    return true;
}

// midFuze: Middle fuze flag
bool Bomb::setSlotMidFuze(base::Boolean* const p)
{
    setMidFuze( p->asBool() );
    return true;
}

// tailFuze: Tail fuze flag
bool Bomb::setSlotTailFuze(base::Boolean* const p)
{
    setTailFuze( p->asBool() );
    return true;
}

// fuzeAltitude:  Fuze arming Altitude
bool Bomb::setSlotFuzeAltitude(base::Number* const p)
{
    setFuzeAltitude( p->asDouble() );
    return true;
}

// fuzeTime:  Fuze Arming Time
bool Bomb::setSlotFuzeTime(base::Number* const p)
{
    setFuzeTime( p->asDouble() );
    return true;
}

// dragIndex: drag index used by default dynamics
bool Bomb::setSlotDragIndex(base::Number* const p)
{
    setDragIndex( p->asDouble() );
    return true;
}

}
}
