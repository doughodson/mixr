
#ifndef __mixr_models_ExternalStore_H__
#define __mixr_models_ExternalStore_H__

#include "mixr/models/system/System.hpp"

namespace mixr {
namespace base { class String; }
namespace models {

//------------------------------------------------------------------------------
// Class: ExternalStore
//
// Description: Base class for non-Weapon class, external stores.  These may
//              include bomb racks, missile launchers, fuel tanks, avionics pods,
//              and gun pods.
//
// Factory name: ExternalStore
// Slots:
//    type         <base::String>  ! Type string (default: 0)
//    jettisonable <Boolean>       ! Store can be jettisoned (default: true )
//
// Events:
//      JETTISON_EVENT      Jettison event
//
// Notes:
//    1) ExternalStore class objects are usually updated by a parent Stores class
//       object during time-critical phases and the background thread.
//
//    2) ExternalStore objects are not released, but may be jettisoned. If the object
//       if jettisoned, it will no longer be updated.
//
//------------------------------------------------------------------------------
class ExternalStore : public System
{
   DECLARE_SUBCLASS(ExternalStore, System)

public:
   ExternalStore();

   const base::String* getType() const;

   bool isJettisonable() const;  // True if the weapon can be jettisoned
   bool isJettisoned() const;    // True if the weapon has been jettisoned

   // Event handler(s)
   virtual bool onJettisonEvent();

   bool event(const int event, base::Object* const obj = nullptr) override;
   void reset() override;

protected:
   bool setJettisonable(const bool);    // Sets the jettison enable flag

private:
   base::safe_ptr<const base::String> type;   // Type string for external store
   bool canJettison {true};                   // Weapon can be jettisoned.
   bool jettisoned {};                        // Weapon has been jettisoned.

private:
   // slot table helper methods
   bool setSlotType(const base::String* const);
   bool setSlotJettisonable(base::Number* const);
};

}
}

#endif
