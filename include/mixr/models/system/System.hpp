
#ifndef __mixr_models_common_System_HPP__
#define __mixr_models_common_System_HPP__

#include "mixr/base/IComponent.hpp"

namespace mixr {
namespace base { class String; }
namespace models {
class WorldModel;
class Player;

//------------------------------------------------------------------------------
// Class: System
//
// Description: Base class for all models of system components that can be attached
//              to a player.
//
// Factory name: System
// Slots:
//    powerSwitch    <base::String>   ! Power switch position ("OFF", "STBY", "ON") (default: "ON")
//
//
// Events:
//    KILL_EVENT        ()       Process a (we were just) killed events
//
//
// Notes:
//
//    1) Time critical phase Callbacks -- The following virtual member functions
//       are to be overridden by derived systems, as needed.  Each function is
//       called one per frame during the corresponding phase.
//
//      virtual void dynamics(const double dt);    // Phase 0
//      virtual void transmit(const double dt);    // Phase 1
//      virtual void receive(const double dt);     // Phase 2
//      virtual void process(const double dt);     // Phase 3
//
//    2) Power switch enums --
//       a) Current switch enums are PWR_OFF, PWR_STBY and PWR_ON.
//       b) PWR_LAST is use by derived systems to expand the power switch list
//             enum {
//                PWR_NEW1 = BaseClass::PWR_LAST,
//                PWR_NEW2,
//                PWR_LAST
//             };
//       c) derived systems can extend the 'powerSwitch' slot, setSlotPowerSwitch(),
//          for additional switch positions.
//
//    3) Systems inherit the SHUTDOWN_EVENT event from Component.  So, make sure
//       to use this to shutdown threads and network connections that you may
//       have created, and to unref() objects that can't wait until deleteData()
//       (e.g., circular references).  (see Component::shutdownNotification())
//
//------------------------------------------------------------------------------
class System : public base::IComponent
{
   DECLARE_SUBCLASS(System, base::IComponent)

public:
   // Power switch enumerations (can be expanded by derived classes)
   enum {
      PWR_OFF,
      PWR_STBY,
      PWR_ON,        // On, Normal, Operate, etc.
      PWR_LAST       // Hook for subclasses to expand
   };

public:
   System();

   virtual unsigned int getPowerSwitch() const;          // Returns the system's master power switch setting (see power enumeration)
   virtual bool setPowerSwitch(const unsigned int p);    // Sets the system's master power switch setting (see power enumeration)

   // Event handler(s)
   virtual bool killedNotification(Player* const killedBy = nullptr); // Killed (KILL_EVENT) event handler

   virtual Player* getOwnship();                         // Returns a pointer to our ownship player
   virtual const Player* getOwnship() const;             // Returns a pointer to our ownship player (const version)

   void updateData(const double dt = 0.0) override;
   void updateTC(const double dt = 0.0) override;
   bool event(const int event, base::Object* const obj = nullptr) override;
   void reset() override;
   bool isFrozen() const override;

protected:
   virtual WorldModel* getWorldModel();
   virtual const WorldModel* getWorldModel() const;

   // Time critical phase callbacks --
   // --- to be used by the derived classes, as needed
   virtual void dynamics(const double dt);    // Phase 0
   virtual void transmit(const double dt);    // Phase 1
   virtual void receive(const double dt);     // Phase 2
   virtual void process(const double dt);     // Phase 3

private:
   bool findOwnship();

   Player* ownship {};           // Our player (not ref()'d because the own player owns us).
   unsigned int pwrSw {PWR_ON};  // System's master power switch

private:
   // slot table helper methods
   bool setSlotPowerSwitch(const base::String* const);
};

}
}

#endif
