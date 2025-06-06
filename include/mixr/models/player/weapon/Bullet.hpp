
#ifndef __mixr_models_common_Bullet_HPP__
#define __mixr_models_common_Bullet_HPP__

#include "mixr/models/player/weapon/IWeapon.hpp"
#include <array>

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Bullet
//
// Description: General bullet class.
//
//    Provides a description of the bullet.  It is used to create the "flyout"
//    weapon player.  During flyout, the bullets are grouped into bursts.
//
// Factory name: Bullet
//------------------------------------------------------------------------------
class Bullet : public IWeapon
{
    DECLARE_SUBCLASS(Bullet, IWeapon)

public:
   static const double DEFAULT_MUZZLE_VEL;         // Meters / second
   static const double DEFAULT_MAX_TOF;            // Seconds

public:
   Bullet();

   double getMuzzleVelocity() const                { return muzzleVel; }

   // Fire (add) a burst of bullets
   virtual bool burstOfBullets(
      const base::Vec3d* const pos,     // Initial burst position
      const base::Vec3d* const vel,     // Initial velocity of bullets
      const int num,                    // Number of bullets in burst
      const int rate,                   // Rate of fire (rounds per minute)
      const int re                      // Release event id
   );

   const char* getDescription() const override;
   const char* getNickname() const override;
   int getCategory() const override;

   void reset() override;

protected:
   virtual void resetBurstTrajectories();
   virtual void updateBurstTrajectories(const double dt);
   virtual bool checkForTargetHit();

   IPlayer* getHitPlayer()                 { return hitPlayer; }
   const IPlayer* getHitPlayer() const     { return hitPlayer; }
   void setHitPlayer(IPlayer*);

   void weaponDynamics(const double dt) override;
   void updateTOF(const double dt) override;

   bool shutdownNotification() override;

   struct Burst {
      enum class Status { ACTIVE, HIT, MISS };
      Burst() : bPos(0,0,0), bVel(0,0,0) {}
      base::Vec3d bPos;                // Burst positions -- world  (m)
      base::Vec3d bVel;                // Burst velocities -- world (m)
      double bTof{};                   // Burst time of flight      (sec)
      int    bNum{};                   // Number of rounds in burst
      int    bRate{};                  // Round rate for this burst (rds per min)
      int    bEvent{};                 // Release event number for burst
      Status bStatus{Status::ACTIVE};  // Burst status
   };

private:
   enum { MBT = 100 };         // Max number of burst trajectories

   double muzzleVel {DEFAULT_MUZZLE_VEL}; // Muzzle velocity (m/s)
   base::safe_ptr<IPlayer> hitPlayer;     // Player we hit (if any)

   // Bullet trajectories
   int nbt {};                     // Number of burst trajectories
   std::array<Burst, MBT> bursts;  // Bursts
};

}
}

#endif
