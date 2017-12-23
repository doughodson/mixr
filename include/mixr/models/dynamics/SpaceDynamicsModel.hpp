
#ifndef __mixr_models_SpaceDynamicsModel_H__
#define __mixr_models_SpaceDynamicsModel_H__

#include "mixr/models/dynamics/DynamicsModel.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class SpaceDynamicsModel
// Description:  External player dynamics model
//
// Note --
//    1) updateTC() and updateData() are called by Component::updateTC() and
//    Component::updateData() as part of the player's component list.
//
//    2) computer model dynamics in DynamicsModel::dynamics(), which is called
//    from Player::dynamics() to be synchronized.
//
// Factory name: SpaceDynamicsModel
//------------------------------------------------------------------------------
class SpaceDynamicsModel : public DynamicsModel
{
   DECLARE_SUBCLASS(SpaceDynamicsModel, DynamicsModel)

public:
   SpaceDynamicsModel();

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
