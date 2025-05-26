
#include "mixr/models/player/air/AirVehicle.hpp"

#include "mixr/models/dynamics/IAeroDynamics.hpp"
#include "mixr/base/IList.hpp"
#include "mixr/base/Identifier.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(AirVehicle, "AirVehicle")
EMPTY_DELETEDATA(AirVehicle)

BEGIN_SLOTTABLE(AirVehicle)
    "initGearPos",      // 1: initial gear position (default: up)
END_SLOTTABLE(AirVehicle)

BEGIN_SLOT_MAP(AirVehicle)
    ON_SLOT(1, setSlotInitGearPos, base::Identifier)
END_SLOT_MAP()

AirVehicle::AirVehicle()
{
    STANDARD_CONSTRUCTOR()

    static base::String generic("GenericAirVehicle");
    setType_old(&generic);
    setType("GenericAirVehicle");
}

void AirVehicle::copyData(const AirVehicle& org, const bool)
{
   BaseClass::copyData(org);

   initGearPos = org.initGearPos;
   gearPos = org.gearPos;
   wpnBayDoorPos = org.wpnBayDoorPos;
   wingSweep = org.wingSweep;
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int AirVehicle::getMajorType() const
{
    return AIR_VEHICLE;
}

//------------------------------------------------------------------------------
// reset() -- Reset handler
//------------------------------------------------------------------------------
void AirVehicle::reset()
{
   // reset our base class
   BaseClass::reset();

   if (isLocalPlayer()) {
      // reset the gear switch
      setGearHandleSwitch(initGearPos);
      if (initGearPos > 0) gearPos = 100.0;
      else gearPos = 0.0;
   }
}

//------------------------------------------------------------------------------
// Data access functions that need conversion
//------------------------------------------------------------------------------
double AirVehicle::getAngleOfAttackD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getAngleOfAttack() );
}

double AirVehicle::getFlightPathD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getFlightPath() );
}

double AirVehicle::getSideSlipD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getSideSlip() );
}

IAeroDynamics* AirVehicle::getDynamics()
{
   return dynamic_cast<IAeroDynamics*>( getDynamicsModel() );
}

const IAeroDynamics* AirVehicle::getDynamics() const
{
   return dynamic_cast<const IAeroDynamics*>( getDynamicsModel() );
}

//-----------------------------------------------------------------------------
// Slot functions
//-----------------------------------------------------------------------------

// Set initial gear position by name: up, down
bool AirVehicle::setSlotInitGearPos(const base::Identifier* const x)
{
   bool ok {};
   if (x != nullptr) {
      if (*x == "up" || *x == "UP") {
         initGearPos = 0.0;
         gearPos = 0.0;
         ok = true;
      } else if (*x == "down" || *x == "DOWN") {
         initGearPos = 1.0;
         gearPos = 100.0;
         ok = true;
      }
   }
   if (ok) {
      setGearHandleSwitch(initGearPos);
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "AirVehicle::setInitGearPos(): ERROR -- invalid position identifier; should be up or down" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
double AirVehicle::getGload() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getGload();
   }
   return value;
}

double AirVehicle::getMach() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) value = aero->getMach();
   else value = BaseClass::getMach();
   return value;
}

double AirVehicle::getAngleOfAttack() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getAngleOfAttack();
   }
   return value;
}

double AirVehicle::getSideSlip() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getSideSlip();
   }
   return value;
}

double AirVehicle::getFlightPath() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getFlightPath();
   }
   return value;
}

double AirVehicle::getFuelWt() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getFuelWt();
   }
   return value;
}

double AirVehicle::getFuelWtMax() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getFuelWtMax();
   }
   return value;
}

double AirVehicle::getGrossWeight() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getGrossWeight();
   }
   return value;
}

double AirVehicle::getCalibratedAirspeed() const
{
   double value {getTotalVelocityKts()};      // Default: use total velocity
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getCalibratedAirspeed();
   }
   return value;
}

double AirVehicle::getAmbientPressureRatio() const
{
   double value {1.0};         // Default: 1.0
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getAmbientPressureRatio();
   }
   return value;
}

//------------------------------------------------------------------------------
// setTrimSwitchRollInput(RollTrim) --  Sets the trim switch positions:
//      RollTrim:  -1.0 -> Left;      0.0 -> Hold;  1.0 -> Right
//------------------------------------------------------------------------------
void AirVehicle::setTrimSwitchRollInput(const double value)
{
   IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      aero->setTrimSwitchRollInput(value);
   }
}

//------------------------------------------------------------------------------
// setTrimSwitchPitchInput(PitchTrim) --  Sets the trim switch positions:
//      PitchTrim: -1.0 -> NoseDown;  0.0 -> Hold;  1.0 -> NoseUp
//------------------------------------------------------------------------------
void AirVehicle::setTrimSwitchPitchInput(const double value)
{
   IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      aero->setTrimSwitchPitchInput(value);
   }
}

//------------------------------------------------------------------------------
// setRudderPedalInput(pedal) -- Pedal inputs: normalized
//          pedal:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
void AirVehicle::setRudderPedalInput(const double value)
{
   IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      aero->setRudderPedalInput(value);
   }
}

//------------------------------------------------------------------------------
// setBrakes() -- Sets brake positions (left & right)
//               No brake force  -> 0.0
//               Max brake force -> 1.0
//------------------------------------------------------------------------------
void AirVehicle::setBrakes(const double left, const double right)
{
   IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      aero->setBrakes(left, right);
   }
}

//------------------------------------------------------------------------------
// setFlaps() --
//      Commanded flap position from retracted (0.0) to fully extended (1.0)
//------------------------------------------------------------------------------
void AirVehicle::setFlaps(const double value)
{
   IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      aero->setFlaps(value);
   }
}

//------------------------------------------------------------------------------
// getLandingGearPosition() --   Returns the landing gear position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
double AirVehicle::getLandingGearPosition() const
{
   double value {gearPos};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getLandingGearPosition();
   }
   return value;
}

//------------------------------------------------------------------------------
// getWeaponBayDoorPosition() --   Returns the weapon bay door position (percent)
//      0-> closed;  100.0 -> Fully Open
//------------------------------------------------------------------------------
double AirVehicle::getWeaponBayDoorPosition() const
{
   double value {wpnBayDoorPos};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getWeaponBayDoorPosition();
   }
   return value;
}

//------------------------------------------------------------------------------
// isWeightOnWheels() --   Returns true if there is weight on the wheels.
//------------------------------------------------------------------------------
bool AirVehicle::isWeightOnWheels() const
{
   bool flg {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      flg = aero->isWeightOnWheels();
   }
   return flg;
}

//------------------------------------------------------------------------------
// setGearHandleSwitch() -- Set Gear handle switch position
//                          0 -> handle up;  1 -> handle down
//------------------------------------------------------------------------------
void AirVehicle::setGearHandleSwitch(const double value)
{
   if (value > 0) gearPos = 100.0;
   else gearPos = 0;

   IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      aero->setGearHandleSwitch(value);
   }
}

//------------------------------------------------------------------------------
// getSpeedBrakesSwitch() --   Returns the speed brake position (percent)
//      0-> Fully Retracted;  100.0 -> Fully Extended
//------------------------------------------------------------------------------
double AirVehicle::getSpeedBrakePosition() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getSpeedBrakePosition();
   }
   return value;
}

//------------------------------------------------------------------------------
// getWingSweepAngle() --   Returns the wing sweep position angle (radians)
//------------------------------------------------------------------------------
double AirVehicle::getWingSweepAngle() const
{
   double value {wingSweep};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getWingSweepAngle();
   }
   return value;
}

//------------------------------------------------------------------------------
// setSpeedBrakesSwitch() --   Sets the speed brake switch position:
//      -1.0 -> Retract;  0.0 -> Hold;  1.0 -> Extend
//------------------------------------------------------------------------------
void AirVehicle::setSpeedBrakesSwitch(const double value)
{
   IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      aero->setSpeedBrakesSwitch(value);
   }
}

//------------------------------------------------------------------------------
// setWeaponBayDoorSwitch() --   Sets the weapon bay door switch position:
//        0.0 -> closed; 1.0 -> open
//------------------------------------------------------------------------------
void AirVehicle::setWeaponBayDoorSwitch(const double value)
{
   if (value > 0) wpnBayDoorPos = 100.0;
   else wpnBayDoorPos = 0;

   IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      aero->setWeaponBayDoorSwitch(value);
   }
}

//------------------------------------------------------------------------------
// Sets the commanded wing sweep angle (radians)
//------------------------------------------------------------------------------
void AirVehicle::setCmdWingSweepAngle(const double value)
{
   wingSweep = value;

   IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      aero->setCmdWingSweepAngle(value);
   }
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int AirVehicle::getNumberOfEngines() const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getNumberOfEngines();
   }
   return n;
}

int AirVehicle::getEngThrust(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngThrust(data, max);
   }
   return n;
}

int AirVehicle::getEngThrustIdle(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngThrustIdle(data, max);
   }
   return n;
}

int AirVehicle::getEngThrustMil(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngThrustMil(data, max);
   }
   return n;
}

int AirVehicle::getEngThrustAb(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngThrustAb(data, max);
   }
   return n;
}

int AirVehicle::getEngRPM(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngRPM(data, max);
   }
   return n;
}

int AirVehicle::getEngN1(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngN1(data, max);
   }
   return n;
}

int AirVehicle::getEngPLA(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngPLA(data, max);
   }
   return n;
}

int AirVehicle::getEngFuelFlow(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngFuelFlow(data, max);
   }
   return n;
}

int AirVehicle::getEngOilPressure(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngOilPressure(data, max);
   }
   return n;
}

int AirVehicle::getEngInletTemp(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngInletTemp(data, max);
   }
   return n;
}

int AirVehicle::getEngNozzle(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngNozzle(data, max);
   }
   return n;
}

//------------------------------------------------------------------------------
// Radar altimeter
//------------------------------------------------------------------------------
bool AirVehicle::isRadarAltValid() const
{
    double ra {getRadarAltitude()};
    return (ra >= 0 && ra <= 5000);
}

double AirVehicle::getRadarAltitude() const
{
    return static_cast<double>(getAltitudeAglFt());
}

}
}
