
#ifndef __mixr_models_SimpleStoresMgr_H__
#define __mixr_models_SimpleStoresMgr_H__

#include "mixr/models/system/StoresMgr.hpp"

namespace mixr {
namespace base { class Boolean; class String; }
namespace models {
class AbstractWeapon;
class Bomb;
class Missile;
class Track;
class Chaff;
class Flare;
class Decoy;

//------------------------------------------------------------------------------
// Class: SimpleStoresMgr
// Description: Simple (example) external stores management systems
//
// Factory name: StoresMgr
//
// StoresMgr Events:
//    WPN_REL_EVENT     ! Weapon release switch (see StoresMgr)
//
// Notes:
//    1) The various weapon release functions will return a pre-ref()'d pointer
//       to the flyout weapon, or zero if unable to release a weapon.
//
//------------------------------------------------------------------------------
class SimpleStoresMgr : public StoresMgr
{
    DECLARE_SUBCLASS(SimpleStoresMgr, StoresMgr)

public:
   SimpleStoresMgr();

   int getCurrentWeaponID() const;                       // Current weapon ID number
   int getNumCurrentWeapons() const;                     // Number of weapons with current weapon ID (e.g. # MK84 bombs on the platform)

   // Get the first missile of type weaponType from our weapons list (Pre-ref()'d)
   virtual Missile* getSpecificMissile(const base::String* const missileType);

   // Get the first bomb of type weaponType from our weapons list (Pre-ref()'d)
   virtual Bomb* getSpecificBomb(const base::String* const bombType);

   // Get the first weapon by type from our weapons list (Pre-ref()'d)
   virtual AbstractWeapon* getSpecificWeapon(const std::type_info& type);

   // Get the target track
   virtual Track* getNextTarget();

   virtual bool setWeaponReleaseTimer(const double v);   // Sets the release timer

   virtual bool onWpnRelEvent(const base::Boolean* const sw = 0) override;
   virtual bool onTriggerSwEvent(const base::Boolean* const sw = 0) override;

   virtual AbstractWeapon* getCurrentWeapon() override;
   virtual const AbstractWeapon* getCurrentWeapon() const override;
   virtual bool isWeaponReleased() const override;
   virtual Missile* getNextMissile() override;
   virtual const Missile* getNextMissile() const override;
   virtual Sam* getNextSam() override;
   virtual const Sam* getNextSam() const override;
   virtual Bomb* getNextBomb() override;
   virtual const Bomb* getNextBomb() const override;
   virtual Chaff* getNextChaff() override;
   virtual const Chaff* getNextChaff() const override;
   virtual Flare* getNextFlare() override;
   virtual const Flare* getNextFlare() const override;
   virtual Decoy* getNextDecoy() override;
   virtual const Decoy* getNextDecoy() const override;
   virtual Missile* releaseOneMissile() override;
   virtual Sam* releaseOneSam() override;
   virtual Bomb* releaseOneBomb() override;
   virtual Chaff* releaseOneChaff() override;
   virtual Flare* releaseOneFlare() override;
   virtual Decoy* releaseOneDecoy() override;

   virtual void updateData(const double dt = 0.0) override;

protected:
   virtual void process(const double dt) override;

private:
   Missile* getNextMissileImp();        // First available missile from our weapons list (Pre-ref()'d) (const version)
   Sam* getNextSamImp();                // First available SAM from our weapons list (Pre-ref()'d) (const version)
   Bomb* getNextBombImp();              // First available bomb from our weapons list (Pre-ref()'d) (const version)
   Chaff* getNextChaffImp();            // First available chaff bundle from our weapons list (Pre-ref()'d) (const version)
   Flare* getNextFlareImp();            // First available flare from our weapons list (Pre-ref()'d) (const version)
   Decoy* getNextDecoyImp();            // First available decoy from our weapons list (Pre-ref()'d) (const version)

   double wpnRelTimer {};     // Weapon released timer
   int    curWpnID {};        // Current Weapon ID
   int    nCurWpn {};         // Number of weapons of the current type
};

}
}

#endif
