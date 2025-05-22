
#ifndef __mixr_models_common_OnboardComputer_HPP__
#define __mixr_models_common_OnboardComputer_HPP__

#include "mixr/models/system/ISystem.hpp"

namespace mixr {
   namespace base {
      class PairStream;
   }
namespace models {
class IAction;
class Track;
class ITrackMgr;

//------------------------------------------------------------------------------
// Class: OnboardComputer
// Description: Base class for all Onboard computer systems
//
//    Use to model the avionics packages, and uses the system of systems
//    pattern, which allows computer models to manage other subsystem
//    computer models.
//
//    The on-board computer model is also responsible for managing the various
//    track managers.
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.hpp).
//
// Factory name: OnboardComputer
//------------------------------------------------------------------------------
class OnboardComputer : public ISystem
{
    DECLARE_SUBCLASS(OnboardComputer, ISystem)

public:
   OnboardComputer();

   // Track managers
   virtual ITrackMgr* getTrackManagerByType(const std::type_info& type);
   virtual const ITrackMgr* getTrackManagerByType(const std::type_info& type) const;
   virtual ITrackMgr* getTrackManagerByName(const char* const name);
   virtual const ITrackMgr* getTrackManagerByName(const char* const) const;

   // Air to Air functions
   virtual Track* getNextTarget();                                      // Get the target track
   virtual int getShootList(Track* tlist[], const int max);
   virtual int getShootList(const Track* tlist[], const int max) const;
   virtual void updateShootList(const bool step = false);              // Updates the shoot list
   virtual bool requestNextToShoot(const Track* const nts);            // Request a track to shoot next

   // Trigger an action
   virtual void triggerAction(IAction* const act);

   // Legacy function (will be removed in a future major release)
   virtual int getShootList(base::safe_ptr<Track>* const tlist, const int max);
   virtual int getShootList(base::safe_ptr<const Track>* const tlist, const int max) const;

   void reset() override;
   void updateData(const double dt = 0.0) override;

protected:
   virtual void setNextToShoot(Track* const p);

   // Manage the active action
   virtual void actionManager(const double dt);

   void process(const double dt) override;

   bool shutdownNotification() override;

private:
   base::safe_ptr<IAction> action; // Current steerpoint action
   Track* nextToShoot {};          // Next to shoot track
};

}
}

#endif
