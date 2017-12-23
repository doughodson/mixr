
#ifndef __mixr_models_Bomb_H__
#define __mixr_models_Bomb_H__

#include "mixr/models/player/weapon/AbstractWeapon.hpp"

namespace mixr {
namespace base { class Identifier; }
namespace models {

//------------------------------------------------------------------------------
// Class: Bomb
// Description: Base class for bombs
//
//    Contains a simple ballistics and guidance model, which derived models can
//    override at will.
//
// Factory name: Bomb
// Slots:
//    arming         <Identifier>  ! Bomb arming option (default: NONE)
//    noseFuze       <Number>      ! Nose fuze flag (default: false)
//    midFuze        <Number>      ! Middle fuze flag (default: false)
//    tailFuze       <Number>      ! Tail fuze flag (default: 0.0f)
//    fuzeAltitude   <Number>      ! Fuze arming Altitude (default: 0.0f)
//    fuzeTime       <Number>      ! Fuze Arming Time (default: 0.0f)
//    dragIndex      <Number>      ! drag index used by default dynamics (default: 0.0006f)
//
//------------------------------------------------------------------------------
class Bomb : public AbstractWeapon
{
   DECLARE_SUBCLASS(Bomb, AbstractWeapon)

public:
   // Arming options
   enum { NONE, FREE_FALL, RETARDED, TIMER, PROXIMITY };

public:
   Bomb();

   virtual unsigned int getArmingOption() const;
   virtual bool isArmingOption(const unsigned int a) const;

   virtual double getFuzeAltitude() const;
   virtual double getFuzeTime() const;
   virtual double getDragIndex() const;

   virtual bool isNoseFuze() const;
   virtual bool isMidFuze() const;
   virtual bool isTailFuze() const;

   // Predict bomb's TOF and final position.
   // -- positions and ground plane are relative to a common ref point.
   virtual bool impactPrediction(
      const base::Vec3d* const initPos, // Initial position (meters)
      const base::Vec3d* const initVel, // Initial velocity (meters per sec)
      const double groundPlane,         // Ground plane (meters)
      const double dt,                  // Time step (sec)
      const double maxTime,             // Max TOF (sec)
      base::Vec3d* const finalPos,      // Final position (meters)
      double* const tof                 // TOF (sec)
   ) const;

   // Predict bomb's TOF and final position.
   // -- positions and ground plane are relative to a common ref point.
   static bool weaponImpactPrediction(
      const base::Vec3d* const initPos, // Initial position (meters)
      const base::Vec3d* const initVel, // Initial velocity (meters per sec)
      const double groundPlane,         // Ground plane (meters)
      const double dt,                  // Time step (sec)
      const double maxTime,             // Max TOF (sec)
      const double dragIndex,           // Drag index
      base::Vec3d* const finalPos,      // Final position (meters)
      double* const tof                 // TOF (sec)
   );

   const char* getDescription() const override;
   const char* getNickname() const override;
   int getCategory() const override;

protected:
   virtual bool setNoseFuze(const bool);
   virtual bool setMidFuze(const bool);
   virtual bool setTailFuze(const bool);
   virtual bool setArmingOption(const unsigned int);
   virtual bool setFuzeAltitude(const double);
   virtual bool setFuzeTime(const double);
   virtual bool setDragIndex(const double);

   void weaponGuidance(const double dt) override;
   void weaponDynamics(const double dt) override;

private:
   void initData();

   double dragIndex {0.0006};     // Drag Index

   // Default fuze, guidance & ballistic values for generic bomb
   // (derived models will override these values)

   // common guidance stuff
   base::Vec3d missDistRef;       // Miss distance vector (ref sys)  (meters)
   base::Vec3d tgtRangeRef;       // Target range vector  (ref sys)  (meters)
   double     cmdStrAz {};        // Commanded steering azimuth   (radians)
   double     cmdStrEl {};        // Commanded steering elevation (radians)
   bool       guidanceValid {};   // is guidance data valid.

   // Common arming stuff
   unsigned int arming {NONE};    // Arming option
   bool noseFuze {};              // Nose fuze flag
   bool midFuze {};               // Middle fuze flag
   bool tailFuze {};              // Tail fuze flag
   double fuzeAlt {};             // Fuze arming Altitude
   double fuzeTime {};            // Fuze Arming Time

private:
   // slot table helper methods
   bool setSlotArmingOption(base::Identifier* const);
   bool setSlotNoseFuze(base::Number* const);
   bool setSlotMidFuze(base::Number* const);
   bool setSlotTailFuze(base::Number* const);
   bool setSlotFuzeAltitude(base::Number* const);
   bool setSlotFuzeTime(base::Number* const);
   bool setSlotDragIndex(base::Number* const);
};

}
}

#endif
