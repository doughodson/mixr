
#include "mixr/models/player/effect/Effect.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/Identifier.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/numeric/Number.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Effect, "Effect")
EMPTY_DELETEDATA(Effect)

BEGIN_SLOTTABLE(Effect)
    "dragIndex",        // 1: drag index used by default dynamics
END_SLOTTABLE(Effect)

BEGIN_SLOT_MAP(Effect)
    ON_SLOT(1, setSlotDragIndex, base::Number)
END_SLOT_MAP()

// Weapon data for general bomb
int Effect::getCategory() const                { return GRAVITY; }
const char* Effect::getDescription() const     { return "Generic Effect"; }
const char* Effect::getNickname() const        { return "Effects"; }

Effect::Effect()
{
    STANDARD_CONSTRUCTOR()

    static base::String generic("Effect");
    setType(&generic);

    // Default: unguided, ballistic
    setMaxTOF(10.0);
    setLethalRange(50.0f);
    setMaxBurstRng(500.0f);
    setTSG(9999.0f);
    setSOBT(9999.0f);
    setEOBT(0.0f);
}

void Effect::copyData(const Effect& org, const bool)
{
    BaseClass::copyData(org);
    dragIndex = org.dragIndex;
}

//------------------------------------------------------------------------------
// crashNotification() -- We just hit the ground (need to check fusing and all that jazz)
//------------------------------------------------------------------------------
bool Effect::crashNotification()
{
    const bool ok{killedNotification()};
    setDetonationResults(DETONATE_NONE);
    setMode(DETONATED);
    return ok;
}

//------------------------------------------------------------------------------
// collisionNotification() -- We just impacted with another player (need to check fusing and all that jazz)
//------------------------------------------------------------------------------
bool Effect::collisionNotification(Player* const p)
{
    const bool ok{killedNotification(p)};
    setMode(DETONATED);
    setDetonationResults(DETONATE_NONE);
    return ok;
}

//------------------------------------------------------------------------------
// updateTOF -- default time of flight
//------------------------------------------------------------------------------
void Effect::updateTOF(const double dt)
{
   // As long as we're active ...
   if (isMode(ACTIVE)) {

      // update time of flight,
      setTOF( getTOF() + dt );

      // and check for the end of the flight
      if (getTOF() >= getMaxTOF()) {
         setMode(DETONATED);
         setDetonationResults(DETONATE_NONE);
         return;
      }
   }
}

//------------------------------------------------------------------------------
// weaponDynamics -- default dynamics; using Robot Aircraft (RAC) dynamics
//------------------------------------------------------------------------------
void Effect::weaponDynamics(const double dt)
{
   // Useful constant
   static const double g{base::ETHG * base::distance::FT2M};      // Acceleration of Gravity (m/s/s)

   // ---
   // Compute & Set acceleration vector (earth)
   // ---

   // First drag
   const base::Vec3d tmp{getVelocity() * (-dragIndex)};

   // then gravity
   base::Vec3d ae1{tmp};
   ae1[IDOWN] += g;

   setAcceleration(ae1);

   // ---
   // Compute & set new velocity vector (earth)
   // ---
   base::Vec3d ve1{getVelocity() + (ae1 * dt)};
   setVelocity(ve1);

   // ---
   // .. Only after setVelocity has been called ...
   // ---
   double vp{getTotalVelocity()};
   double vg{getGroundSpeed()};

   // ---
   // Set velocity vector (body)
   //  (total velocity is along X)
   // ---
   setVelocityBody(vp, 0.0, 0.0);

   // ---
   // Sent angular values
   // ---
   const double newPsi{std::atan2(ve1[IEAST],ve1[INORTH])};
   const double newTheta{std::atan2( -ve1[IDOWN], vg )};
   setEulerAngles(0.0, newTheta, newPsi);
   setAngularVelocities(0.0, 0.0, 0.0);
}


//------------------------------------------------------------------------------
// Set Slot routines --
//------------------------------------------------------------------------------

// dragIndex: drag index used by default dynamics
bool Effect::setSlotDragIndex(base::Number* const p)
{
    setDragIndex( p->getReal() );
    return true;
}

}
}
