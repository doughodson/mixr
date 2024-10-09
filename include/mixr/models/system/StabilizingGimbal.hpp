
#ifndef __mixr_models_common_StabilizingGimbal_HPP__
#define __mixr_models_common_StabilizingGimbal_HPP__

#include "mixr/models/system/Gimbal.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: StabilizingGimbal
// Description: This gimbal tries to counter the player's roll, pitch and yaw movements.
// Factory name: StabilizingGimbal
// Slots:
//    stabilizingMode  <base::String>    ! Sets the type of stabilization we desire  (elevation, roll, horizon)
//                                       ! (default: HORIZON)
//------------------------------------------------------------------------------
class StabilizingGimbal : public Gimbal
{
    DECLARE_SUBCLASS(StabilizingGimbal, Gimbal)

public:
    enum StabilizingMode { ELEVATION, ROLL, HORIZON };
    enum MountPosition { NOSE, TAIL, LEFT_WING, RIGHT_WING };

public:
   StabilizingGimbal();

   StabilizingMode getStabilizingMode() const                  { return stabilizingMode; }
   virtual bool setStabilizingMode(const StabilizingMode m);

   MountPosition getMountPosition() const                      { return mountPosition; }
   virtual bool setMountPosition(const MountPosition m);

protected:
   virtual void stabilizingController(const double dt = 0.0);
   virtual void rollStabilizingController(const double dt = 0.0);
   virtual void elevationStabilizingController(const double dt = 0.0);

   void dynamics(const double dt) override;

private:
   StabilizingMode stabilizingMode {HORIZON};  // Gimbal stabilization mode
   MountPosition mountPosition {NOSE};         // Gimbal Mount position

private:
   // slot table helper methods
   bool setSlotStabilizingMode(base::String* const);
   //bool setSlotMountPosition(base::String* const);
};

}
}

#endif
