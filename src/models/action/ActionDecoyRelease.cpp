
#include "mixr/models/action/ActionDecoyRelease.hpp"

#include "mixr/models/player/weapon/Bomb.hpp"
#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/system/Sar.hpp"
#include "mixr/models/system/IStoresMgr.hpp"
#include "mixr/models/navigation/Steerpoint.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/IComponent.hpp"
#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/qty/lengths.hpp"
#include "mixr/base/util/nav_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ActionDecoyRelease, "ActionDecoyRelease")
EMPTY_DELETEDATA(ActionDecoyRelease)

BEGIN_SLOTTABLE(ActionDecoyRelease)
   "numToLaunch",    //  1) number of decoys to launch
   "interval",       //  2) time, in seconds, between launches
END_SLOTTABLE(ActionDecoyRelease)

BEGIN_SLOT_MAP(ActionDecoyRelease)
    ON_SLOT(1, setSlotNumToLaunch, base::Integer)
    ON_SLOT(2, setSlotInterval,    base::INumber)
END_SLOT_MAP()

ActionDecoyRelease::ActionDecoyRelease()
{
   STANDARD_CONSTRUCTOR()
}

void ActionDecoyRelease::copyData(const ActionDecoyRelease& org, const bool)
{
   BaseClass::copyData(org);
   interval = org.interval;
   numToLaunch = org.numToLaunch;
   tod = org.tod;
   startTOD = org.startTOD;
}

//------------------------------------------------------------------------------
//Default function: Launch a decoy
//------------------------------------------------------------------------------
bool ActionDecoyRelease::trigger(OnboardComputer* const mgr)
{
   bool ok{};

   if (mgr != nullptr) {
      IPlayer* own{static_cast<IPlayer*>(mgr->findContainerByType(typeid(IPlayer)))};
      if (own != nullptr) {
         IStoresMgr* sms{own->getStoresManagement()};
         if (sms != nullptr) {
         if (numToLaunch > 1) {
                if (interval == 0.0) {
                    // launch all at once
                    ok = true;
                    for (int i = 0; i < numToLaunch; i++) {
                        sms->releaseOneDecoy();
                    }
                }
                else {
                    // release one, then start counting
                    sms->releaseOneDecoy();
                    tod = own->getWorldModel()->getSimTimeOfDay();
                    startTOD = tod;
                    // decrease our number to launch
                    numToLaunch--;
                    // set our route manager
                    setManager(mgr);
                }
            }
            else {
                ok = true;
                // only one to launch, then just launch one!
                sms->releaseOneDecoy();
            }
         }
      }
   }

   // do we need to launch more at a certain interval?  If so, we aren't complete yet!

   BaseClass::setCompleted(ok);

   return ok;
}

void ActionDecoyRelease::process(const double)
{
    // keep counting until we have our "interval" of seconds
    OnboardComputer* mgr{getManager()};
    if (mgr != nullptr) {
        IPlayer* own{static_cast<IPlayer*>(mgr->findContainerByType(typeid(IPlayer)))};
        if (own != nullptr) {
            tod = own->getWorldModel()->getSimTimeOfDay();
            if (interval < (tod - startTOD)) {
                // we have hit our interval, release another decoy
                IStoresMgr* sms = own->getStoresManagement();
                if (sms != nullptr) {
                    sms->releaseOneDecoy();
                    numToLaunch--;
                    if (numToLaunch == 0) {
                        setManager(nullptr);
                        BaseClass::setCompleted(true);
                    }
                    else {
                        // keep going, start counting again!
                        startTOD = tod;
                    }
                }
            }
        }
    }
}

bool ActionDecoyRelease::setSlotNumToLaunch(const base::Integer* x)
{
    bool ok{};
    if (x != nullptr) {
        ok = setNumToLaunch(x->asInt());
    }
    return ok;
}
bool ActionDecoyRelease::setSlotInterval(const base::INumber* x)
{
    bool ok{};
    if (x != nullptr) {
        ok = setInterval(x->asDouble());
    }
    return ok;
}

}
}
