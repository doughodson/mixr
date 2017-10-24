//------------------------------------------------------------------------------
//        This is a limited capability:  the stabilizing gimbal can
//        only mounted on the nose or the tail or on the y-azis on
//        the left or right wing.
//------------------------------------------------------------------------------

#include "mixr/models/system/StabilizingGimbal.hpp"

#include "mixr/models/player/Player.hpp"

#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/units/Decibel.hpp"
#include "mixr/base/units/Angles.hpp"
#include "mixr/base/units/Powers.hpp"
#include "mixr/base/osg/Vec3d"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(StabilizingGimbal, "StabilizingGimbal")
EMPTY_DELETEDATA(StabilizingGimbal)

BEGIN_SLOTTABLE(StabilizingGimbal)
    "stabilizingMode",      //  Sets the type of stabilization we desire  (elevation, roll, horizon)
END_SLOTTABLE(StabilizingGimbal)

BEGIN_SLOT_MAP(StabilizingGimbal)
    ON_SLOT(1, setSlotStabilizingMode, base::String);
END_SLOT_MAP()

StabilizingGimbal::StabilizingGimbal()
{
   STANDARD_CONSTRUCTOR()
}

void StabilizingGimbal::copyData(const StabilizingGimbal& org, const bool)
{
   BaseClass::copyData(org);

   stabilizingMode = org.stabilizingMode;
   mountPosition = org.mountPosition;;
}

//------------------------------------------------------------------------------
// dynamics() -- System class "Dynamics phase" call back
//------------------------------------------------------------------------------
void StabilizingGimbal::dynamics(const double dt)
{
   stabilizingController(dt);

   // Call BaseClass after to scan controller since the servo controller
   // is located in BaseClass.
   BaseClass::dynamics(dt);
}

//------------------------------------------------------------------------------
// stabilizingController() -- control the gimbal scanning
//------------------------------------------------------------------------------
void StabilizingGimbal::stabilizingController(const double dt)
{
    switch (stabilizingMode) {
        case ROLL : { rollStabilizingController(dt); }
            break;

        case ELEVATION : { elevationStabilizingController(dt); }
            break;

        case HORIZON : { rollStabilizingController(dt);  elevationStabilizingController(dt);}
            break;
    }
}

//------------------------------------------------------------------------------
// rollStabilizingController() -- Roll stabilizes the gimbal
//------------------------------------------------------------------------------
void StabilizingGimbal::rollStabilizingController(const double)
{
    if (getOwnship() == nullptr) return;

    base::Vec3d tpos{getCmdPosition()};
    if (mountPosition == NOSE){
        tpos[ROLL_IDX] = static_cast<double>(-getOwnship()->getRoll());
    }
    else if (mountPosition == TAIL){
        tpos[ROLL_IDX] = static_cast<double>(getOwnship()->getRoll());
    }
    else if (mountPosition == RIGHT_WING){
        tpos[ELEV_IDX] = static_cast<double>(-getOwnship()->getPitch());
    }
    else if (mountPosition == LEFT_WING){
        tpos[ELEV_IDX] = static_cast<double>(getOwnship()->getPitch());
    }
    setCmdPos( tpos );
}

//------------------------------------------------------------------------------
// elevationStabilizingController() -- Elevation stabilizes the gimbal
//------------------------------------------------------------------------------
void StabilizingGimbal::elevationStabilizingController(const double)
{
    if (getOwnship() == nullptr) return;

    base::Vec3d tpos{getCmdPosition()};
    if (mountPosition == NOSE){
        tpos[ELEV_IDX] = static_cast<double>(-getOwnship()->getPitch());
    }
    else if (mountPosition == TAIL){
        tpos[ELEV_IDX] = static_cast<double>(getOwnship()->getPitch());
    }
    else if (mountPosition == RIGHT_WING){
        tpos[ROLL_IDX] = static_cast<double>(-getOwnship()->getRoll());
    }
    else if (mountPosition == LEFT_WING){
        tpos[ROLL_IDX] = static_cast<double>(getOwnship()->getRoll());
    }
    setCmdPos( tpos );
}

//------------------------------------------------------------------------------
// setStabilizingMode() - sets the gimbal scan pattern
//------------------------------------------------------------------------------
bool StabilizingGimbal::setStabilizingMode(const StabilizingMode m)
{
   stabilizingMode = m;
   return true;
}

//------------------------------------------------------------------------------
// setStabilizingMode() - sets the gimbal scan pattern
//------------------------------------------------------------------------------
bool StabilizingGimbal::setMountPosition(const MountPosition m)
{
   mountPosition = m;
   return true;
}

//------------------------------------------------------------------------------
// setSlotStabilizingMode() -- calls setStabalizingMode()
//------------------------------------------------------------------------------
bool StabilizingGimbal::setSlotStabilizingMode(base::String* const msg)
{
    // set our scan mode
    bool ok{true};
    if (msg != nullptr) {
        if (*msg == "roll") ok = setStabilizingMode(ROLL);
        else if (*msg == "elevation") ok = setStabilizingMode(ELEVATION);
        else if (*msg == "horizon") ok = setStabilizingMode(HORIZON);
        else ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMountPosition() -- calls setMountPosition()
//------------------------------------------------------------------------------
/*
bool StabilizingGimbal::setSlotMountPosition(base::String* const msg)
{
    // set our scan mode
    bool ok = true;
    if (msg != nullptr) {
        if (*msg == "nose") ok = setMountPosition(NOSE);
        else if (*msg == "tail") ok = setMountPosition(TAIL);
        else if (*msg == "left") ok = setMountPosition(LEFT_WING);
        else if (*msg == "right") ok = setMountPosition(RIGHT_WING);
        else ok = false;
    }
    return ok;
}
*/

}
}
