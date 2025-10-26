
#include "mixr/models/player/weapon/Bullet.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/base/IList.hpp"
#include "mixr/base/IPairStream.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Bullet, "Bullet")
EMPTY_SLOTTABLE(Bullet)

// Default Parameters
const double Bullet::DEFAULT_MUZZLE_VEL {1000.0};     // Meters / second
const double Bullet::DEFAULT_MAX_TOF {3.0};           // Seconds

int Bullet::getCategory() const               { return (GRAVITY); }
const char* Bullet::getDescription() const    { return "Bullets"; }
const char* Bullet::getNickname() const       { return "Bullets"; }

Bullet::Bullet()
{
   STANDARD_CONSTRUCTOR()

   static base::String generic("Bullet");
   setType_old(&generic);
   setType("Bullet");

   setMaxTOF( DEFAULT_MAX_TOF );
}

void Bullet::copyData(const Bullet& org, const bool)
{
   BaseClass::copyData(org);

   nbt = 0;
   hitPlayer = nullptr;

   for (int i = 0; i < MBT; i++) {
      bursts[i].bPos = org.bursts[i].bPos;
      bursts[i].bVel = org.bursts[i].bVel;
      bursts[i].bTof = org.bursts[i].bTof;
      bursts[i].bNum = org.bursts[i].bNum;
      bursts[i].bRate = org.bursts[i].bRate;
      bursts[i].bEvent = org.bursts[i].bEvent;
   }
}

void Bullet::deleteData()
{
   setHitPlayer(nullptr);
}

bool Bullet::shutdownNotification()
{
   setHitPlayer(nullptr);
   return BaseClass::shutdownNotification();
}

void Bullet::reset()
{
   BaseClass::reset();
   resetBurstTrajectories();
   setHitPlayer(nullptr);
}

//------------------------------------------------------------------------------
// weaponDynamics() -- Bullet dynamics
//------------------------------------------------------------------------------
void Bullet::weaponDynamics(const double dt)
{
   if (isMode(Mode::ACTIVE)) {
      updateBurstTrajectories(dt);
      checkForTargetHit();

      // This weapon is slaved to the first burst!
      if (nbt > 0) {

         // We control the position and altitude!
         setPosition( bursts[0].bPos[0], bursts[0].bPos[1], bursts[0].bPos[2], true );

         setVelocity( bursts[0].bVel );

         setAcceleration( 0, 0, 0 );

         setEulerAngles(  0, 0, getGroundTrack() );

         setAngularVelocities( 0, 0, 0 );

         setVelocityBody ( bursts[0].bVel.length(), 0, 0 );
      }
   }
}

//------------------------------------------------------------------------------
// updateTOF -- update time of flight (TOF) and set the missed status
//------------------------------------------------------------------------------
void Bullet::updateTOF(const double)
{
   // As long as we're active ...
   if (isMode(Mode::ACTIVE)) {

      // count the number of active bursts and remove aged bullet bursts.
      int n{};
      int nhits{};
      for (int i = 0; i < nbt; i++) {
         if (bursts[i].bStatus == Burst::Status::ACTIVE) {
            n++;
            if ( bursts[i].bTof >= getMaxTOF() ) {
               bursts[i].bStatus = Burst::Status::MISS;
            }
         } else if (bursts[i].bStatus == Burst::Status::HIT) {
            nhits++;
         }
      }

      // If we have no active bursts .. we've detonated (so to speak)
      if (n == 0) {
         setMode(Mode::DETONATED);
         // final detonation results (hit or miss) are located with each burst ...
         if (nhits > 0) {
            setDetonationResults( Detonation::ENTITY_IMPACT );
         } else {
            setDetonationResults( Detonation::NONE );
         }
         // final time of flight (slave to the first burst)
         setTOF( bursts[0].bTof );
      }
   }
}

//------------------------------------------------------------------------------
// resetBulletTrajectories() -- reset the bullet trajectories
//------------------------------------------------------------------------------
void Bullet::resetBurstTrajectories()
{
   nbt = 0;
}

//------------------------------------------------------------------------------
// burstOfBullets() -- add a burst of bullets
//------------------------------------------------------------------------------
bool Bullet::burstOfBullets(const base::Vec3d* const pos, const base::Vec3d* const vel, const int num, const int rate, const int e)
{
   if (nbt < MBT && pos != nullptr && vel != nullptr) {
      bursts[nbt].bPos = *pos;  // Burst positions -- world  (m)
      bursts[nbt].bVel = *vel;  // Burst velocities -- world (m)
      bursts[nbt].bTof = 0;    // Burst time of flight      (sec)
      bursts[nbt].bNum = num;  // Number of rounds in burst
      bursts[nbt].bRate = rate;// Round rate for this burst (rds per sec)
      bursts[nbt].bEvent = e;  // Release event number for burst
      bursts[nbt].bStatus = Burst::Status::ACTIVE;
      nbt++;
   }
   return true;
}

//------------------------------------------------------------------------------
// updateBurstTrajectories() -- update the bursts trajectories
//------------------------------------------------------------------------------
void Bullet::updateBurstTrajectories(const double dt)
{
   static const double g{base::ETHG * base::length::FT2M};      // Acceleration of Gravity (m/s/s)

   // For all active bursts
   for (int i = 0; i < nbt; i++) {
      if (bursts[i].bStatus == Burst::Status::ACTIVE) {
         bursts[i].bVel[IPlayer::IDOWN] = bursts[i].bVel[IPlayer::IDOWN] + (g*dt);  // falling bullets

         bursts[i].bPos = bursts[i].bPos + (bursts[i].bVel * dt);
         bursts[i].bTof += dt;
      }
   }
}

//------------------------------------------------------------------------------
// checkForTargetHit() -- check to see if we hit anything
//------------------------------------------------------------------------------
bool Bullet::checkForTargetHit()
{
   IPlayer* ownship{getLaunchVehicle()};
   IPlayer* tgt{getTargetPlayer()};
   if (ownship != nullptr && tgt != nullptr) {
      base::Vec3d osPos{tgt->getPosition()};

      // For all active bursts ...
      for (int i = 0; i < nbt; i++) {
         if (bursts[i].bStatus == Burst::Status::ACTIVE) {

            // Check if we're within range of the target
            base::Vec3d rPos{bursts[i].bPos - osPos};
            double rng{rPos.length()};
            if (rng < 10.0) {
               // Yes -- it's a hit!
               bursts[i].bStatus = Burst::Status::HIT;
               setHitPlayer(tgt);
               setLocationOfDetonation();
               tgt->processDetonation(rng,this);
            }

         }
      }
   }
   // if we are just flying along, check our range to the nearest player and tell him we killed it
   else {
        //base::Vec3 old = getEulerAngles();
        base::Vec3d myPos{getPosition()};
        base::Vec3d tgtPos;
        base::Vec3d vecPos;
        //double az{};
        //double el{};
        double range{};
        //double diffAz{}0;
        //double diffEl{};
        double maxRange{1.0};                  // close range of detonation
        WorldModel* sim{getWorldModel()};
        if (sim != nullptr) {
            base::IPairStream* players{sim->getPlayers()};
            if (players != nullptr) {
                base::IList::Item* item{players->getFirstItem()};
                while (item != nullptr) {
                    const auto pair = static_cast<base::Pair*>(item->getValue());
                    if (pair != nullptr) {
                        const auto player = dynamic_cast<IPlayer*>(pair->object());
                        if (player != nullptr && player != ownship && player->isMajorType(LIFE_FORM) && !player->isDestroyed()) {
                            // ok, calculate our position from this guy
                            tgtPos = player->getPosition();
                            vecPos = tgtPos - myPos;
                            //az = std::atan2(vecPos.y(), vecPos.x());
                            range = (vecPos.x() * vecPos.x() + vecPos.y() * vecPos.y());
                            range = std::sqrt(range);
                            if (range < maxRange) {
                                // tell this target we hit it
                                player->processDetonation(range, this);
                            }
                        }
                    }
                    item = item->getNext();
                }
                players->unref();
                players = nullptr;
            }
        }

   }
   return false;
}

//------------------------------------------------------------------------------
// setHitPlayer() -- set a pointer to the player we just hit
//------------------------------------------------------------------------------
void Bullet::setHitPlayer(IPlayer* p)
{
   hitPlayer = p;
}

}
}
