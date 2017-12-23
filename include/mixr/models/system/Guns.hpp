
#ifndef __mixr_models_Gun_H__
#define __mixr_models_Gun_H__

#include "mixr/models/system/ExternalStore.hpp"

#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Matrixd"

namespace mixr {
namespace base { class List; class Number; }
namespace models {
class Bullet;

//------------------------------------------------------------------------------
// Class: Gun
// Description: Base class for guns; with a simple, default gun model
//
//    The Gun class is derived from the ExternalStore class because it's a system
//    that's attached to the aircraft.  It is not released and can not become an
//    independent player, and it is updated by the background and time-critical
//    threads.
//
// Factory name: Gun
// Slots:
//    bulletType  <Bullet>   ! Type of bullet (have to have bullets to work) (default: 0)
//    rounds      <Number>   ! Number of rounds (default: 510)
//    unlimited    <Number>   ! Unlimited rounds flag (default: false)
//    rate        <Number>   ! Rate of fire (rounds per minute) (default: 6600)
//    burstRate   <Number>   ! Rate to generate small bursts of bullets (default: 10)
//
//    The following are used to position the gun on the ownship player
//    position    <List>     ! Position vector; relative to ownship axis [ nose right down ]  (meters)
//    roll        <Number>   ! roll angle; relative to ownship axis (radians, base::Angle) (default: 0)
//    pitch       <Number>   ! pitch; relative to ownship axis (radians, base::Angle) (default: 0)
//    yaw         <Number>   ! heading; relative to ownship axis (radians, base::Angle) (default: 0)
//
//------------------------------------------------------------------------------
class Gun : public ExternalStore
{
   DECLARE_SUBCLASS(Gun, ExternalStore)

public:
   static const int DEFAULT_ROUNDS_PER_MINUTE;  // Default rate (rds per min)
   static const int DEFAULT_NUM_ROUNDS;         // Default load
   static const int DEFAULT_BURST_RATE;         // Default subburst frame rate

public:
   Gun();

   int getRoundsRemaining() const                  { return rounds; }

   int getMaxRounds() const                        { return initRounds; }
   virtual bool setMaxRounds(const int value);

   bool isUnlimited() const                        { return unlimited; }
   virtual bool setUnlimited(const bool flg);

   int getRoundsPerMinute() const                  { return rpm; }
   virtual bool setRoundsPerMinute(const int value);

   // Gun arming
   bool isGunArmed() const                         { return armed; }
   virtual bool setGunArmed(const bool flg);

   // Gun fire
   bool isGunFiring() const                         { return fire; }
   virtual bool fireControl(const bool trigger, const bool burst = false);

   // Reload the gun & bullets
   virtual void reload();
   Bullet* getBulletType()                         { return bullet; }
   const Bullet* getBulletType() const             { return bullet; }
   virtual bool setBulletType(Bullet* const b);

   // Gun position relative to ownship player
   const base::Vec3d& getPosition() const          { return posVec; }
   const base::Vec3d& getAngles() const            { return angles; }
   const base::Matrixd& getRotMat() const          { return rm; }
   double getRoll() const                          { return angles[0]; }
   double getPitch() const                         { return angles[1]; }
   double getYaw() const                           { return angles[2]; }
   virtual void setPitch(const double x)           { angles[1] = x; }
   bool setPosition(const double x, const double y, const double z);
   bool setAngles(const double r, const double p, const double y);

   void reset() override;

protected:
   virtual double computeBulletRatePerSecond();

   virtual void initBullet();
   virtual void burstFrame();
   virtual base::Vec3d computeInitBulletPosition();
   virtual base::Vec3d computeInitBulletVelocity();

   void process(const double dt) override;

   bool shutdownNotification() override;

private:
   Bullet*  bullet {};             // Type of bullet
   double   shortBurstTimer {};    // Single short burst timer (when firing a single burst) (sec)
   double   shortBurstTime {0.5};  // Length of the single short burst                 (sec)
   double   burstFrameTimer {};    // Timer used to generate small bursts bullets      (sec)
   double   burstFrameTime {};     // Length of a small burst of bullets               (sec)
   double   rcount {};             // Rounds being fired this burst
   int      rounds {};             // Number of rounds remaining
   int      initRounds {};         // Initial rounds
   int      rpm {};                // Rounds per minute
   bool     fire {};               // Gun is firing
   bool     armed {};              // Armed flag
   bool     unlimited {};          // Unlimited bullets (aka old western movie mode)

   base::Vec3d posVec;        // Position vector; relative to ownship body axis (m)
   base::Vec3d angles;        // (Euler) angles; relative to ownship body axis (rad)
   base::Matrixd rm;          // Rotational Matrix; relative to ownship body axis

private:
   // slot table helper methods
   bool setSlotNumRounds(const base::Number* const);  // Number of rounds
   bool setSlotUnlimited(const base::Number* const);  // Unlimited rounds flag
   bool setSlotRate(const base::Number* const);       // Rate of fire (rds per min)
   bool setSlotBurstRate(const base::Number* const);  // Burst rate
   bool setSlotPosition(base::List* const);           // Gun position relative to ownship
   bool setSlotRoll(const base::Number* const);       // Gun roll angle to ownship
   bool setSlotPitch(const base::Number* const);      // Gun pitch angle to ownship
   bool setSlotYaw(const base::Number* const);        // Gun heading angle to ownship
};

}
}

#endif
