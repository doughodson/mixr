
#ifndef __mixr_models_common_TargetData_HPP__
#define __mixr_models_common_TargetData_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base { class IAngle; class Boolean; class Identifier; class Integer; class ILength;
                 class INumber; class String; class ITime; }
namespace models {

//------------------------------------------------------------------------------
// Class: TargetData
// Description: General purpose target profile data that can be associated
//              with a target steerpoint or a weapon.
//
// Factory name: TargetData
// Slots:
//    enabled          <Boolean>        ! Target enabled flag (default: false)
//    completed        <Boolean>        ! Target completed flag (default: false)
//    weaponType       <String>         ! Type of weapon to release (default: 0)
//    quantity         <Integer>        ! Number of weapons to release (default: 0)
//    manualAssign     <Boolean>        ! Manually assign weapon to target? (default: false)
//    stickType        <Identifier>     ! Weapon stick option (MIDPOINT, LEADING_EDGE) (default: MIDPOINT)
//
//    stickDistance    <ILength>        ! Weapon stick length (default: 0)
//    stickDistance    <INumber>        ! (feet)
//
//    interval         <ITime>          ! Time between weapon releases (default: 0)
//    interval         <INumber>        ! (MSec)
//
//    maxMissDistance  <ILength>        ! Maximum miss distance (default: 0)
//    maxMissDistance  <INumber>        ! (feet)
//
//    armDelay         <ITime>          ! Arming delay (default: 0)
//    armDelay         <INumber>        ! (0-99.9 sec)
//
//    angle            <IAngle>         ! Impact angle (default: 0)
//    angle            <INumber>        ! (degrees)
//
//    azimuth          <IAngle>         ! Azimuth angle (default: 0)
//    azimuth          <INumber>        ! (degrees)
//
//    velocity         <INumber>        ! Impact velocity (ft/sec) (default: 0)
//------------------------------------------------------------------------------
class TargetData : public base::Object
{
   DECLARE_SUBCLASS(TargetData, base::Object)

public:
   // 'Stick' types
   static const unsigned int MIDPOINT{};
   static const unsigned int LEADING_EDGE{1};

public:
    TargetData();

    bool isEnabled() const;
    bool isCompleted() const;
    const base::String* getWpnType() const;
    unsigned int getQuantity() const;
    unsigned int getStickType() const;
    double getStickDistance() const;      // Feet
    double getMaxMissDistance() const;    // Feet
    double getInterval() const;           // Milli-Seconds
    bool getManualAssign() const;
    double getArmDelay() const;           // Seconds
    double getAngle() const;              // Degrees
    double getAzimuth() const;            // Degrees
    double getVelocity() const;           // Ft/Sec

    bool setEnabled(const bool b);
    bool setCompleted(const bool b);
    bool setWpnType(const base::String* const s);
    bool setQuantity(const unsigned int q);
    bool setStickType(const unsigned int t);
    bool setStickDistance(const double ft);
    bool setMaxMissDistance(const double ft);
    bool setInterval(const double ms);
    bool setManualAssign(const bool b);
    bool setArmDelay(const double sec);
    bool setAngle(const double degs);
    bool setAzimuth(const double degs);
    bool setVelocity(const double fps);

private:
    bool enabled {};
    bool completed {};
    const base::String* wpnType {};

    double angle {};          // degs
    double azimuth {};        // degs

    double armDelay {};       // sec
    double velocity {};       // ft/sec
    double stickDist {};      // ft
    double maxMissDist {};    // ft
    double interval {};       // MS

    unsigned int quantity {};
    unsigned int stickType {MIDPOINT};
    bool manualAssign {};

private:
   bool setSlotEnabled(const base::Boolean* const);
   bool setSlotCompleted(const base::Boolean* const);
   bool setSlotWpnType(const base::String* const);
   bool setSlotQuantity(const base::Integer* const);
   bool setSlotManualAssign(const base::Boolean* const);
   bool setSlotStickType(const base::Identifier* const);
   bool setSlotStickDistance(const base::ILength* const);
   bool setSlotStickDistance(const base::INumber* const);
   bool setSlotInterval(const base::ITime* const);
   bool setSlotInterval(const base::INumber* const);
   bool setSlotMaxMissDistance(const base::ILength* const);
   bool setSlotMaxMissDistance(const base::INumber* const);
   bool setSlotArmDelay(const base::ITime* const);
   bool setSlotArmDelay(const base::INumber* const);
   bool setSlotAngle(const base::IAngle* const);
   bool setSlotAngle(const base::INumber* const);
   bool setSlotAzimuth(const base::IAngle* const);
   bool setSlotAzimuth(const base::INumber* const);
   bool setSlotVelocity(const base::INumber* const);
};

inline bool TargetData::isEnabled() const                    { return enabled; }
inline bool TargetData::isCompleted() const                  { return completed; }
inline const base::String* TargetData::getWpnType() const    { return wpnType; }
inline unsigned int TargetData::getQuantity() const          { return quantity; }
inline unsigned int TargetData::getStickType() const         { return stickType; }
inline double TargetData::getStickDistance() const           { return stickDist; }
inline double TargetData::getMaxMissDistance() const         { return maxMissDist; }
inline double TargetData::getInterval() const                { return interval; }
inline bool TargetData::getManualAssign() const              { return manualAssign; }
inline double TargetData::getArmDelay() const                { return armDelay; }
inline double TargetData::getAngle() const                   { return angle; }
inline double TargetData::getAzimuth() const                 { return azimuth; }
inline double TargetData::getVelocity() const                { return velocity; }

}
}

#endif
