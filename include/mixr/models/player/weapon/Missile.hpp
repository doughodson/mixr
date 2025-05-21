
#ifndef __mixr_models_common_Missile_HPP__
#define __mixr_models_common_Missile_HPP__

#include "mixr/models/player/weapon/IWeapon.hpp"

namespace mixr {
namespace base { class INumber; }
namespace models {
class IPlayer;
class Track;

//------------------------------------------------------------------------------
// Class: Missile
// Description: Base class for missiles; with a simple, default missile model
//
// Factory name: Missile
// Slots:
//   minSpeed     <INumber>  ! Minimum Velocity (m/s)  (default: 0.0)
//   maxSpeed     <INumber>  ! Maximum Velocity (m/s)  (default: 800.0)
//   speedMaxG    <INumber>  ! Velocity we reach max G (default: 800.0)
//   maxg         <INumber>  ! Max G's (at "speedMaxG" or above) (default: 4.0)
//   maxAccel     <INumber>  ! Maximum Acceleration (m/s/s) (default: 50.0)
//   cmdPitch     <INumber>  ! Command Pitch (rad) (default: 0.0)
//   cmdHeading   <INumber>  ! Command Heading (rad) (default: 0.0)
//   cmdSpeed     <INumber>  ! Command speed (m/s) (default: 0.0)
//
//------------------------------------------------------------------------------
class Missile : public IWeapon
{
    DECLARE_SUBCLASS(Missile, IWeapon)

public:
    Missile();

    // get functions
    double getVpMin() const         { return vpMin; }
    double getVpMax() const         { return vpMax; }
    double getVpMaxG() const        { return vpMaxG; }
    double getMaxG() const          { return maxG; }
    double getMaxAccel() const      { return maxAccel; }
    double getMinAccel() const      { return 0.0; }

    // set functions
    bool setVpMin(const double v);
    bool setVpMax(const double v);
    bool setVpMaxG(const double v);
    bool setMaxG(const double v);
    bool setMaxAccel(const double v);

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
    void atReleaseInit() override;

    virtual void setCmdPitchD(const double x)  { cmdPitch   = x * static_cast<double>(base::angle::D2RCC); }
    virtual void setCmdHdgD(const double x)    { cmdHeading = x * static_cast<double>(base::angle::D2RCC); }

    bool setTargetTrack(Track* const trk, const bool posTrkEnb) override;
    bool setTargetPlayer(IPlayer* const tgt, const bool posTrkEnb) override;

    bool event(const int event, base::IObject* const obj = nullptr) override;
    void reset() override;

protected:
   virtual bool setSlotVpMin(const base::INumber* const msg);
   virtual bool setSlotVpMax(const base::INumber* const msg);
   virtual bool setSlotVpMaxG(const base::INumber* const msg);
   virtual bool setSlotMaxG(const base::INumber* const msg);
   virtual bool setSlotMaxAccel(const base::INumber* const msg);
   virtual bool setSlotCmdPitch(const base::INumber* const msg);
   virtual bool setSlotCmdHeading(const base::INumber* const msg);
   virtual bool setSlotCmdVelocity(const base::INumber* const msg);

   // Weapon interface
   void weaponGuidance(const double dt) override;
   void weaponDynamics(const double dt) override;

private:
    virtual bool calculateVectors(const IPlayer* const tgt, const Track* const trk, base::Vec3d* const los, base::Vec3d* const vel, base::Vec3d* const posx) const;

   // ---
   // Default guidance & dynamics parameters
   // ---
   double trng {};          // target range             (m)
   double trngT {};         // target range (truth)     (m)
   double trdot {};         // target range rate        (m/s)
   double trdotT {};        // target range rate (truth)(m/s)
   double cmdPitch {};      // Commanded Pitch          (rad)
   double cmdHeading {};    // Commanded Heading        (rad)
   double cmdVelocity {};   // Commanded speed          (m/s)
   double vpMin {};         // Minimum Velocity         (m/s)
   double vpMax {};         // Maximum Velocity         (m/s)
   double maxAccel {};      // Max longitudual acceleration ((f/s)/s)
   double maxG {};          // Max lateral G's (pitch/yaw)  (gees)
   double vpMaxG {};        // Velocity for Max G's     (gees)
};

}
}

#endif
