
#include "mixr/models/player/effect/IEffect.hpp"

#include "mixr/base/IList.hpp"
#include "mixr/base/Identifier.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/numeric/INumber.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IEffect, "IEffect")
EMPTY_DELETEDATA(IEffect)

BEGIN_SLOTTABLE(IEffect)
    "dragIndex",        // 1: drag index used by default dynamics
END_SLOTTABLE(IEffect)

BEGIN_SLOT_MAP(IEffect)
    ON_SLOT(1, setSlotDragIndex, base::INumber)
END_SLOT_MAP()

// Weapon data for general bomb
int IEffect::getCategory() const                { return GRAVITY; }
const char* IEffect::getDescription() const     { return "Generic Effect"; }
const char* IEffect::getNickname() const        { return "Effects"; }

IEffect::IEffect()
{
    STANDARD_CONSTRUCTOR()

    static base::String generic("Effect");
    setType_old(&generic);
    setType("Effect");

    // Default: unguided, ballistic
    setMaxTOF(10.0);
    setLethalRange(50.0f);
    setMaxBurstRng(500.0f);
    setTSG(9999.0f);
    setSOBT(9999.0f);
    setEOBT(0.0f);
}

void IEffect::copyData(const IEffect& org, const bool)
{
    BaseClass::copyData(org);
    dragIndex = org.dragIndex;
}

//------------------------------------------------------------------------------
// crashNotification() -- We just hit the ground (need to check fusing and all that jazz)
//------------------------------------------------------------------------------
bool IEffect::crashNotification()
{
    const bool ok{killedNotification()};
    setDetonationResults(Detonation::NONE);
    setMode(Mode::DETONATED);
    return ok;
}

//------------------------------------------------------------------------------
// collisionNotification() -- We just impacted with another player (need to check fusing and all that jazz)
//------------------------------------------------------------------------------
bool IEffect::collisionNotification(IPlayer* const p)
{
    const bool ok{killedNotification(p)};
    setMode(Mode::DETONATED);
    setDetonationResults(Detonation::NONE);
    return ok;
}

//------------------------------------------------------------------------------
// updateTOF -- default time of flight
//------------------------------------------------------------------------------
void IEffect::updateTOF(const double dt)
{
   // As long as we're active ...
   if (isMode(Mode::ACTIVE)) {

      // update time of flight,
      setTOF( getTOF() + dt );

      // and check for the end of the flight
      if (getTOF() >= getMaxTOF()) {
         setMode(Mode::DETONATED);
         setDetonationResults(Detonation::NONE);
         return;
      }
   }
}

//------------------------------------------------------------------------------
// weaponDynamics -- default dynamics; using Robot Aircraft (RAC) dynamics
//------------------------------------------------------------------------------
void IEffect::weaponDynamics(const double dt)
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
bool IEffect::setSlotDragIndex(base::INumber* const p)
{
    setDragIndex( p->asDouble() );
    return true;
}

}
}
