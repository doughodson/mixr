
#ifndef __mixr_models_common_IDynamicsSpace_HPP__
#define __mixr_models_common_IDynamicsSpace_HPP__

#include "mixr/models/dynamics/IDynamics.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class IDynamicsSpace
// Description: Interface to all space dynamics
//
// Note --
//    1) updateTC() and updateData() are called by Component::updateTC() and
//    Component::updateData() as part of the player's component list.
//
//    2) computer model dynamics in DynamicsModel::dynamics(), which is called
//    from Player::dynamics() to be synchronized.
//
// Factory name: IDynamicsSpace
//------------------------------------------------------------------------------
class IDynamicsSpace : public IDynamics
{
   DECLARE_SUBCLASS(IDynamicsSpace, IDynamics)

public:
   IDynamicsSpace();

   // Rotation Stick Input Positions (Roll and Pitch in BaseClass)
   virtual bool setControlStickYawInput(const double);

   // Translate Stick Input Positions
   virtual bool setTranslateXInput(const double);
   virtual bool setTranslateYInput(const double);
   virtual bool setTranslateZInput(const double);
};

}
}

#endif
