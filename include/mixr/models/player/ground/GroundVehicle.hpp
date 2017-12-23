
#ifndef __mixr_models_GroundVehicle_H__
#define __mixr_models_GroundVehicle_H__

#include "mixr/models/player/Player.hpp"

namespace mixr {
namespace base { class Angle; class Identifier; class Time; }
namespace models {

//------------------------------------------------------------------------------
// Class GroundVehicle
//
// Factory name: GroundVehicle
// Slots:
//    commandedPosition <Identifier>     ! Launcher's init commanded position [UP, DOWN, NONE] (default: NONE)
//    launcherDownAngle <Angle>          ! Min (down) Launcher angle (default: 0.0)
//    launcherUpAngle   <Angle>          ! Max (up) Launcher angle (default: PI/2.0)
//    launcherMoveTime  <Time>           ! Max time to move between 'down' and 'up' positions (default: 10.0f)
//
// Notes:
//   (1) The launcher move time should be greater than zero seconds, and the up
//       angle should be greater than the down angle.
//   (2) This launcher is a temp solution -- this will be removed.
//
//------------------------------------------------------------------------------
class GroundVehicle : public Player
{
   DECLARE_SUBCLASS(GroundVehicle, Player)

public:
   enum LauncherCommand { NONE, DOWN, UP };

public:
   GroundVehicle();

   double getGrossWeight() const override;             // wt: lbs

   virtual double getFuelWt() const;                   // wt: lbs
   virtual double getFuelWtMax() const;                // wt: lbs

   virtual double getLauncherPosition() const;         // Launcher elevation angle (rad)
   virtual double getLauncherRate() const;             // Launcher rate (rad/sec)
   virtual LauncherCommand getLauncherCommand() const; // Returns the launcher's command position (UP/DOWN) or NONE if not commanded


   // Commands the launcher to the up/down position
   virtual bool commandLauncher(const LauncherCommand cmd);

   // Sets the launcher elevation angle (rad), and removes the old position command
   virtual bool setLauncherPosition(const double rad);

   unsigned int getMajorType() const override;

   void reset() override;

protected:
   // Launcher dynamics -- moves launcher to its commanded position
   virtual void launcherDynamics(const double dt);

   // Player class (protected) interface
   void dynamics(const double  dt = 0.0) override;

private:
   double            lnchrAngle {};        // Current launcher angle  (rad)
   double            lnchrRate {};         // Rate the launcher is moving (rad/sec)
   double            lnchrDownAngle {};    // Min launcher angle      (rad)
   double            lnchrUpAngle {};      // Max launcher angle      (rad)
   double            lnchrMoveTime {};     // Time to move between min & max angles (sec)
   LauncherCommand   initLnchrPos {NONE};  // Initial launcher position
   LauncherCommand   cmdLnchrPos {NONE};   // Commanded launcher position

private:
   // slot table helper methods
   bool setSlotCommandedPosition(const base::Identifier* const);
   bool setSlotLauncherDownAngle(const base::Angle* const);
   bool setSlotLauncherUpAngle(const base::Angle* const);
   bool setSlotLauncherMoveTime(const base::Time* const);
};

}
}

#endif
