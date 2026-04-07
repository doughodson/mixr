
#include "mixr/models/player/ground/IGroundVehicle.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/IList.hpp"
#include "mixr/base/IPairStream.hpp"

#include "mixr/base/qty/angles.hpp"
#include "mixr/base/qty/times.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IGroundVehicle, "IGroundVehicle")
EMPTY_DELETEDATA(IGroundVehicle)

static const double DEFAULT_LAUNCHER_UP_ANGLE   {static_cast<double>(base::PI/2.0)};  // Default max launcher angle (rad)
static const double DEFAULT_LAUNCHER_DOWN_ANGLE {0.0};                                // Default min launcher angle (rad)
static const double DEFAULT_LAUNCHER_MOVE_TIME  {10.0};                               // Default max launcher movement time (sec)

BEGIN_SLOTTABLE(IGroundVehicle)
    "commandedPosition",      // 1: Launcher's init commanded position [ "up" "down" ] (base::Identifier)
    "launcherDownAngle",      // 2: Min (down) Launcher angle (base::Angle)
    "launcherUpAngle",        // 3: Max (up) Launcher angle (base::Angle)
    "launcherMoveTime",       // 4: Max time to move between 'down' and 'up' positions (base::Time)
END_SLOTTABLE(IGroundVehicle)

BEGIN_SLOT_MAP(IGroundVehicle)
    ON_SLOT(1, setSlotCommandedPosition, base::Identifier)
    ON_SLOT(2, setSlotLauncherDownAngle, base::IAngle)
    ON_SLOT(3, setSlotLauncherUpAngle,   base::IAngle)
    ON_SLOT(4, setSlotLauncherMoveTime,  base::ITime)
END_SLOT_MAP()

IGroundVehicle::IGroundVehicle()
{
   STANDARD_CONSTRUCTOR()
   static base::String generic("GenericGroundVehicle");
   setType_old(&generic);
   setType("GenericGroundVehicle");

   lnchrDownAngle = DEFAULT_LAUNCHER_DOWN_ANGLE;
   lnchrUpAngle   = DEFAULT_LAUNCHER_UP_ANGLE;
   lnchrMoveTime  = DEFAULT_LAUNCHER_MOVE_TIME;

   setTerrainOffset(1.5);      // default offset from terrain to CG
}

void IGroundVehicle::copyData(const IGroundVehicle& org, const bool)
{
   BaseClass::copyData(org);

   lnchrDownAngle = org.lnchrDownAngle;
   lnchrUpAngle = org.lnchrUpAngle;
   lnchrMoveTime  = org.lnchrMoveTime;
   lnchrAngle = org.lnchrAngle;
   lnchrRate = org.lnchrRate;
   initLnchrPos = org.initLnchrPos;
   cmdLnchrPos = org.cmdLnchrPos;
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int IGroundVehicle::getMajorType() const
{
    return GROUND_VEHICLE;
}

//------------------------------------------------------------------------------
// Reset()
//------------------------------------------------------------------------------
void IGroundVehicle::reset()
{
   BaseClass::reset();

   if (isLocalPlayer()) {
      if (initLnchrPos == UP) {
         cmdLnchrPos = UP;
         lnchrAngle = lnchrUpAngle;
         lnchrRate = 0.0;
      } else if (initLnchrPos == DOWN) {
         cmdLnchrPos = DOWN;
         lnchrAngle = lnchrDownAngle;
         lnchrRate = 0.0;
      } else {
         cmdLnchrPos = NONE;
         lnchrAngle = 0.0;
         lnchrRate = 0.0;
      }
   }
}

//------------------------------------------------------------------------------
// dynamics() -- update vehicle dynamics
//------------------------------------------------------------------------------
void IGroundVehicle::dynamics(const double dt)
{
   BaseClass::dynamics(dt);
   launcherDynamics(dt);
}

//------------------------------------------------------------------------------
// Launcher dynamics -- moves launcher to its commanded position
//------------------------------------------------------------------------------
void IGroundVehicle::launcherDynamics(const double dt)
{
   if (lnchrMoveTime > 0 && cmdLnchrPos != NONE) {

      double rate{(lnchrUpAngle - lnchrDownAngle) / lnchrMoveTime};
      double angle{lnchrAngle};

      if (cmdLnchrPos == UP && lnchrAngle != lnchrUpAngle) {
         angle = lnchrAngle + (rate * dt);
         if (angle >= lnchrUpAngle) {
            angle = lnchrUpAngle;
            rate = 0.0;
         }
      } else if (cmdLnchrPos == DOWN && lnchrAngle != lnchrDownAngle) {
         angle = lnchrAngle - (rate * dt);
         if (angle <= lnchrDownAngle) {
            angle = lnchrDownAngle;
            rate = 0.0;
         }
      }

      lnchrAngle = angle;
      lnchrRate = rate;
   }
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------
double IGroundVehicle::getGrossWeight() const
{
    return 0.0;
}

double IGroundVehicle::getFuelWt() const
{
    return 0.0;
}

double IGroundVehicle::getFuelWtMax() const
{
    return 0.0;
}

// Launcher position (rad)
double IGroundVehicle::getLauncherPosition() const
{
   return lnchrAngle;
}

// Launcher rate (rad/sec)
double IGroundVehicle::getLauncherRate() const
{
   return lnchrRate;
}

// Returns the launcher's command position
IGroundVehicle::LauncherCommand IGroundVehicle::getLauncherCommand() const
{
   return cmdLnchrPos;
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool IGroundVehicle::commandLauncher(const LauncherCommand cmd)
{
   cmdLnchrPos = cmd;
   return true;
}

// Sets the launcher elevation angle (rad), and removes the old position command
bool IGroundVehicle::setLauncherPosition(const double rad)
{
   lnchrAngle = rad;
   lnchrRate = 0.0;
   cmdLnchrPos = NONE;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// commandedPosition: Launcher's init commanded position [ "up" "down" ] (base::Identifier)
bool IGroundVehicle::setSlotCommandedPosition(const base::Identifier* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      if (*msg == "up" || *msg == "UP") {
         initLnchrPos = UP;
         cmdLnchrPos = UP;
         ok = true;
      } else if (*msg == "down" || *msg == "DOWN") {
         initLnchrPos = DOWN;
         cmdLnchrPos = DOWN;
         ok = true;
      }
   }
   return ok;
}

// launcherDownAngle: Min (down) Launcher angle (base::Angle)
bool IGroundVehicle::setSlotLauncherDownAngle(const base::IAngle* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      lnchrDownAngle = msg->getValueInRadians();
      ok = true;
   }
   return ok;
}

// launcherUpAngle: Max (up) Launcher angle (base::Angle)
bool IGroundVehicle::setSlotLauncherUpAngle(const base::IAngle* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      lnchrUpAngle = msg->getValueInRadians();
      ok = true;
   }
   return ok;
}

// launcherMoveTime: Max time to move between 'down' and 'up' positions (base::Time)
bool IGroundVehicle::setSlotLauncherMoveTime(const base::ITime* const x)
{
   bool ok{};
   if (x != nullptr) {
      lnchrMoveTime = x->getValueInSeconds();
      ok = true;
   }
   return ok;
}

}
}
