#include "mixr/models/action/ActionCamouflageType.hpp"

#include "mixr/models/player/weapon/Bomb.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/system/Sar.hpp"
#include "mixr/models/system/StoresMgr.hpp"
#include "mixr/models/navigation/Steerpoint.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/IComponent.hpp"
#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/util/nav_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ActionCamouflageType, "ActionCamouflageType")
EMPTY_DELETEDATA(ActionCamouflageType)

BEGIN_SLOTTABLE(ActionCamouflageType)
   "camouflageType",    //  1) User defined camouflage type (positive integer or zero for none)
END_SLOTTABLE(ActionCamouflageType)

BEGIN_SLOT_MAP(ActionCamouflageType)
    ON_SLOT( 1, setSlotCamouflageType, base::Integer)
END_SLOT_MAP()

ActionCamouflageType::ActionCamouflageType()
{
   STANDARD_CONSTRUCTOR()
}

void ActionCamouflageType::copyData(const ActionCamouflageType& org, const bool)
{
   BaseClass::copyData(org);

   camouflage = org.camouflage;
}

//------------------------------------------------------------------------------
// Default function: Change our ownship's camouflage type (one-shot)
//------------------------------------------------------------------------------
bool ActionCamouflageType::trigger(OnboardComputer* const mgr)
{
   bool ok{};

   if (mgr != nullptr) {
      Player* own{static_cast<Player*>(mgr->findContainerByType(typeid(Player)))};
      if (own != nullptr) {
         // Set our ownship's camouflage type
         own->setCamouflageType( getCamouflageType() );
         // that's all folks
         BaseClass::setCompleted(true);
         ok = true;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the camouflage type for this player
bool ActionCamouflageType::setCamouflageType(const unsigned int v)
{
   camouflage = v;
   return true;
}

// Sets user defined camouflage type
bool ActionCamouflageType::setSlotCamouflageType(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int ii{msg->asInt()};
      if (ii >= 0) {
         ok = setCamouflageType( ii );
      }
   }
   return ok;
}

}
}

