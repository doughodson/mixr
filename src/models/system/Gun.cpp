
#include "mixr/models/system/Gun.hpp"

#include "mixr/models/player/weapon/Bullet.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/base/List.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"

#include "mixr/simulation/AbstractDataRecorder.hpp"

#include "mixr/base/util/nav_utils.hpp"

#include "mixr/base/units/angles.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Gun, "Gun")

BEGIN_SLOTTABLE(Gun)
    "bulletType",       //  1: Type of bullet (have have bullets to work)
    "rounds",           //  2: Number of rounds
    "unlimited",        //  3: Unlimited rounds flag
    "rate",             //  4: Rate of fire (rounds per minute)
    "burstRate",        //  5: Rate to generate small bursts of bullets

    // The following are used to position the gun on the ownship player
    "position",         //  6: Position vector; relative to ownship axis [ nose right down ]  (meters)
    "roll",             //  7: roll angle; relative to ownship axis (radians, base::Angle)
    "pitch",            //  8: pitch; relative to ownship axis (radians, base::Angle)
    "yaw"               //  9: heading; relative to ownship axis (radians, base::Angle)
END_SLOTTABLE(Gun)

BEGIN_SLOT_MAP(Gun)
    ON_SLOT(1, setBulletType,     Bullet)
    ON_SLOT(2, setSlotNumRounds,  base::Integer)
    ON_SLOT(3, setSlotUnlimited,  base::Boolean)
    ON_SLOT(4, setSlotRate,       base::Integer)
    ON_SLOT(5, setSlotBurstRate,  base::Integer)
    ON_SLOT(6, setSlotPosition,   base::List)

    ON_SLOT(7, setSlotRoll,       base::Number)
    ON_SLOT(7, setSlotRoll,       base::Angle)

    ON_SLOT(8, setSlotPitch,      base::Number)
    ON_SLOT(8, setSlotPitch,      base::Angle)

    ON_SLOT(9, setSlotYaw,        base::Number)
    ON_SLOT(9, setSlotYaw,        base::Angle)
END_SLOT_MAP()

Gun::Gun()
{
   STANDARD_CONSTRUCTOR()

   burstFrameTime = 1.0 / static_cast<double>(DEFAULT_BURST_RATE);

   initRounds = DEFAULT_NUM_ROUNDS;

   rpm = DEFAULT_ROUNDS_PER_MINUTE;

   setPosition(0.0, 0.0, 0.0);
   setAngles(0.0, 0.0, 0.0);
   // Note: rotation matrix (mm) was initialized by setAngles()
}

void Gun::copyData(const Gun& org, const bool)
{
   BaseClass::copyData(org);

   if (org.getBulletType() != nullptr) {
      Bullet* b{org.getBulletType()->clone()};
      setBulletType( b );
      b->unref();
   } else {
      setBulletType(nullptr);
   }

   armed = org.armed;

   burstFrameTimer = org.burstFrameTimer;
   burstFrameTime = org.burstFrameTime;
   rcount = org.rcount;

   fire = org.fire;
   unlimited = org.unlimited;

   shortBurstTimer = org.shortBurstTimer;
   shortBurstTime  = org.shortBurstTime;

   rounds = org.rounds;
   initRounds = org.initRounds;

   rpm = org.rpm;
}

void Gun::deleteData()
{
   setBulletType(nullptr);
}

bool Gun::shutdownNotification()
{
   if (bullet != nullptr) {
      bullet->event(SHUTDOWN_EVENT);
      bullet->unref();
      bullet = nullptr;
   }

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Gun::reset()
{
   BaseClass::reset();
   if (getBulletType() != nullptr) getBulletType()->reset();
   reload();
}

//------------------------------------------------------------------------------
// reload() -- Reload the gun
//------------------------------------------------------------------------------
void Gun::reload()
{
   rounds = initRounds;
}

//------------------------------------------------------------------------------
// Process our time-critical tasks
//------------------------------------------------------------------------------
void Gun::process(const double dt)
{
   BaseClass::process(dt);

   // ---
   // Are we firing?
   // ---
   if (fire && (getRoundsRemaining() > 0 || isUnlimited()) ) {
      const double rps{computeBulletRatePerSecond()};
      const double bpi{rps * dt};
      rcount += bpi;
   }

   // ---
   // Generate small burst of bullets at 10 hz
   // ---
   burstFrameTimer += dt;
   if (burstFrameTimer >= burstFrameTime) {
      burstFrameTimer = 0;
      if (rcount > 0) burstFrame();
   }

   // ---
   // Burst timer
   // ---
   if (shortBurstTimer > 0 && fire) {
      shortBurstTimer -= dt;
      if (shortBurstTimer <= 0) {
         shortBurstTimer = 0;
         fire = false;
      }
   }
}

//------------------------------------------------------------------------------
// computeBulletRatePerSecond() -- Compute the rate of bullets fired per second
//------------------------------------------------------------------------------
double Gun::computeBulletRatePerSecond()
{
   double spinUp{1.0};
   return (static_cast<double>(rpm) / 60.0f) * spinUp;
}

//------------------------------------------------------------------------------
// fireControl() -- Starts/stops gun firing;
//    'trigger' will start or stop firing;
//    if 'burst' is true, only a single burst is fired;
//    returns true if gun is now firing
//------------------------------------------------------------------------------
bool Gun::fireControl(const bool trigger, const bool burst)
{
   if (trigger && isGunArmed()) {
      // Start firing ...
      fire = true;
      initBullet();
      if (burst) shortBurstTimer = shortBurstTime;
      else shortBurstTimer = 0;
   } else {
      // Stop firing ...
      burstFrame(); // clear the gun of bullets to be fired
      fire = false;
      shortBurstTimer = 0;
   }
   return fire;
}

//------------------------------------------------------------------------------
// setUnlimited() -- Set the unlimited rounds flag
//------------------------------------------------------------------------------
bool Gun::setUnlimited(const bool flg)
{
   unlimited = flg;
   return true;
}

//------------------------------------------------------------------------------
// setMaxRounds() -- Set the maximum rounds (loaded)
//------------------------------------------------------------------------------
bool Gun::setMaxRounds(const int value)
{
   initRounds = value;
   return true;
}

//------------------------------------------------------------------------------
// setRoundsPerMinute() -- Set the gun's rate of fire
//------------------------------------------------------------------------------
bool Gun::setRoundsPerMinute(const int nvalue)
{
   rpm = nvalue;
   return true;
}

//------------------------------------------------------------------------------
// setGunArmed() -- Set the gun's armed flag
//------------------------------------------------------------------------------
bool Gun::setGunArmed(const bool flg)
{
   armed = flg;
   return true;
}

//------------------------------------------------------------------------------
// setBulletType() -- Set the gun's bullet type
//------------------------------------------------------------------------------
bool Gun::setBulletType(Bullet* const b)
{
   if (bullet != nullptr) {
      bullet->unref();
   }
   bullet = b;
   if (bullet != nullptr) {
      bullet->ref();
      bullet->container(this);
   }
   return true;
}

//------------------------------------------------------------------------------
// initBullet() -- init the bullet
//------------------------------------------------------------------------------
void Gun::initBullet()
{
   Bullet* p{getBulletType()};
   if (p != nullptr) p->reset();
}

//------------------------------------------------------------------------------
// burstFrame() -- generate a small burst of bullets (usually every 1/10 seconds)
//------------------------------------------------------------------------------
void Gun::burstFrame()
{
   // Compute the number of rounds this burst
   int ibullets{static_cast<int>(rcount + 0.5f)};

   // Do we have any bullets to fire ...
   if (ibullets > 0){

      // Decrease the number of rounds remaining
      if ( !isUnlimited() ) {
         if (ibullets > rounds) {
            ibullets = rounds;
            rcount = static_cast<double>(ibullets);
         }
         rounds -= ibullets;
      }


      // Update the rounds fired count
      rcount -= ibullets;

      // Log this event
      Player* ownship{static_cast<Player*>( findContainerByType(typeid(Player)) )};

      if (ownship != nullptr) {
         BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_GUN_FIRED )
            SAMPLE_1_OBJECT( ownship )
            SAMPLE_1_VALUE( rcount )
         END_RECORD_DATA_SAMPLE()
      }

      // When we have a bullet model ... we're going to create a bullet (weapon)
      // player to flyout the rounds.
      Bullet* wpn{getBulletType()};
      const auto sim = static_cast<WorldModel*>( findContainerByType(typeid(WorldModel)) );
      if (wpn != nullptr && ownship != nullptr && sim != nullptr) {

         // Compute the bullet burst's initial position and velocity
         base::Vec3d ipos{computeInitBulletPosition()};
         base::Vec3d ivel{computeInitBulletVelocity()};

         // Get the bullet player being used to fly-out the bullets
         Bullet* flyout{static_cast<Bullet*>( wpn->getFlyoutWeapon() )};
         if (flyout == nullptr) {
            // If we don't have the flyout bullet (i.e., weapon and player) ... create it
            wpn->setLaunchVehicle(ownship);
            flyout = static_cast<Bullet*>( wpn->release() );
         }

         // The flyout bullet (player) will handle this burst of bullets.
         if (flyout != nullptr) {
            flyout->burstOfBullets(&ipos, &ivel, ibullets, getRoundsPerMinute(), sim->getNewWeaponEventID() );
         }

         // Cleanup
         if (flyout != nullptr) { flyout->unref(); flyout = nullptr; }

      }
   }
   else rcount = 0;
}

//------------------------------------------------------------------------------
// computeInitBulletPosition() -- compute the initial bullet position
//------------------------------------------------------------------------------
base::Vec3d Gun::computeInitBulletPosition()
{
   base::Vec3d pe1{posVec};
   const auto ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
   if (ownship != nullptr) {
      // Body position to earth (NED) position
      base::Vec3d gunPosE{posVec * ownship->getRotMat()};
      pe1 = gunPosE + ownship->getPosition();
   }
   return pe1;
}

//------------------------------------------------------------------------------
// computeInitBulletVelocity() -- compute the initial bullet position
//------------------------------------------------------------------------------
base::Vec3d Gun::computeInitBulletVelocity()
{
   base::Vec3d ve1(0,0,0);   // velocity -- earth (m/s)
   const auto ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
   if (ownship != nullptr) {
      // compute the earth (NED) to gun matrix
      base::Matrixd mm{getRotMat() * ownship->getRotMat()};

      // Get muzzle velocity in gun axis and transform to earth axis
      double muzzleVel{Bullet::DEFAULT_MUZZLE_VEL};
      if (getBulletType() != nullptr) muzzleVel = getBulletType()->getMuzzleVelocity();
      base::Vec3d va(muzzleVel, 0.0, 0.0);
      ve1 = va * mm;  // same as:  ve1 = mm(T) * va
   }
   return ve1;
}


//------------------------------------------------------------------------------
// setPosition() -- set the gun's local position vector; relative to ownship axis (m)
//------------------------------------------------------------------------------
bool Gun::setPosition(const double x, const double y, const double z)
{
    posVec.set(x, y, z);
    return true;
}

//------------------------------------------------------------------------------
// setAngles() -- set the gun's angles (roll, pitch, yaw) relative to ownship axis (m)
//------------------------------------------------------------------------------
bool Gun::setAngles(const double r, const double p, const double y)
{
    // Set angles
    angles.set(r,p,y);

    // Set rotational matrix
    base::nav::computeRotationalMatrix(getRoll(), getPitch(), getYaw(), &rm);

    return true;
}

// Number of rounds
bool Gun::setSlotNumRounds(const base::Integer* const num)
{
   bool ok{};
   if (num != nullptr) {
      ok = setMaxRounds( num->asInt() );
   }
   return ok;
}

// Unlimited rounds flag
bool Gun::setSlotUnlimited(const base::Boolean* const num)
{
   bool ok{};
   if (num != nullptr) {
      ok = setUnlimited( num->asBool() );
   }
   return ok;
}

// Rate of fire (rds per min)
bool Gun::setSlotRate(const base::Integer* const num)
{
   bool ok{};
   if (num != nullptr) {
      ok = setRoundsPerMinute( num->asInt() );
   }
   return ok;
}

// Burst rate
bool Gun::setSlotBurstRate(const base::Integer* const num)
{
   bool ok{};
   if (num != nullptr) {
      const int rate{num->asInt()};
      if (rate > 0 && rate <= 20) {
         burstFrameTime = 1.0f / static_cast<double>(rate);
         ok = true;
      } else {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Player::setSlotBurstRate: invalid burst rate, valid range is 1 .. 20hz" << std::endl;
         }
      }
   }
   return ok;
}

// Gun position relative to ownship
bool Gun::setSlotPosition(base::List* const numList)
{
   bool ok{};
   double values[3]{};
   const std::size_t n{numList->getNumberList(values, 3)};
   if (n == 3) {
      setPosition(values[0], values[1], values[2]);
      ok = true;
   }
   return ok;
}

// Gun roll angle to ownship
bool Gun::setSlotRoll(const base::Number* const x)
{
   bool ok{};
   double value{-1000.0};

   if (x != nullptr) {
      value = x->asDouble();
   }

   if (value >= -base::PI && value <= base::PI) {
      setAngles( value, getPitch(), getYaw() );
      ok = true;
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Player::setSlotInitRoll: invalid roll angle, valid range is [-pi ... +pi]" << std::endl;
      }
   }
   return ok;
}

// Gun roll angle to ownship
bool Gun::setSlotRoll(const base::Angle* const x)
{
   bool ok{};
   double value{ -1000.0 };

   if (x != nullptr) {
      value = x->getValueInRadians();
   }

   if (value >= -base::PI && value <= base::PI) {
      setAngles(value, getPitch(), getYaw());
      ok = true;
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Player::setSlotInitRoll: invalid roll angle, valid range is [-pi ... +pi]" << std::endl;
      }
   }
   return ok;
}

// Gun pitch angle to ownship
bool Gun::setSlotPitch(const base::Number* const x)
{
   bool ok{};
   double value{-1000.0};

   if (x != nullptr) {
      value = x->asDouble();
   }

   if (value >= -base::PI && value <= base::PI) {
      setAngles( getRoll(), value, getYaw() );
      ok = true;
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Gun::setSlotPitch: invalid pitch angle, valid range is [-pi ... +pi]" << std::endl;
      }
   }
   return ok;
}

// Gun pitch angle to ownship
bool Gun::setSlotPitch(const base::Angle* const x)
{
   bool ok{};
   double value{ -1000.0 };

   if (x != nullptr) {
      value = x->getValueInRadians();
   }

   if (value >= -base::PI && value <= base::PI) {
      setAngles(getRoll(), value, getYaw());
      ok = true;
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Gun::setSlotPitch: invalid pitch angle, valid range is [-pi ... +pi]" << std::endl;
      }
   }
   return ok;
}

// Gun heading angle to ownship
bool Gun::setSlotYaw(const base::Number* const x)
{
   bool ok{};
   double value{-1000.0};

   if (x != nullptr) {
      value = x->asDouble();
   }

   if (value >= -base::PI && value <= 2.0f*base::PI) {
      setAngles( getRoll(), getPitch(), value );
      ok = true;
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Gun::setSlotYaw: invalid yaw angle, valid range is [-pi ... +2*pi]" << std::endl;
      }
   }
   return ok;
}

// Gun heading angle to ownship
bool Gun::setSlotYaw(const base::Angle* const x)
{
   bool ok{};
   double value{ -1000.0 };

   if (x != nullptr) {
      value = x->getValueInRadians();
   }

   if (value >= -base::PI && value <= 2.0f*base::PI) {
      setAngles(getRoll(), getPitch(), value);
      ok = true;
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Gun::setSlotYaw: invalid yaw angle, valid range is [-pi ... +2*pi]" << std::endl;
      }
   }
   return ok;
}

}
}
