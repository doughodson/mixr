
#include "mixr/models/player/air/IAirVehicle.hpp"

#include "mixr/models/dynamics/IAeroDynamics.hpp"
#include "mixr/base/IList.hpp"
#include "mixr/base/Identifier.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IAirVehicle, "IAirVehicle")
EMPTY_DELETEDATA(IAirVehicle)

BEGIN_SLOTTABLE(IAirVehicle)
    "initGearPos",      // 1: initial gear position (default: up)
END_SLOTTABLE(IAirVehicle)

BEGIN_SLOT_MAP(IAirVehicle)
    ON_SLOT(1, setSlotInitGearPos, base::Identifier)
END_SLOT_MAP()

IAirVehicle::IAirVehicle()
{
    STANDARD_CONSTRUCTOR()

    static base::String generic("GenericAirVehicle");
    setType_old(&generic);
    setType("GenericAirVehicle");
}

void IAirVehicle::copyData(const IAirVehicle& org, const bool)
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
unsigned int IAirVehicle::getMajorType() const
{
    return AIR_VEHICLE;
}

//------------------------------------------------------------------------------
// reset() -- Reset handler
//------------------------------------------------------------------------------
void IAirVehicle::reset()
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
double IAirVehicle::getAngleOfAttackD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getAngleOfAttack() );
}

double IAirVehicle::getFlightPathD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getFlightPath() );
}

double IAirVehicle::getSideSlipD() const
{
   return ( static_cast<double>(base::angle::R2DCC) * getSideSlip() );
}

IAeroDynamics* IAirVehicle::getDynamics()
{
   return dynamic_cast<IAeroDynamics*>( getDynamicsModel() );
}

const IAeroDynamics* IAirVehicle::getDynamics() const
{
   return dynamic_cast<const IAeroDynamics*>( getDynamicsModel() );
}

//-----------------------------------------------------------------------------
// Slot functions
//-----------------------------------------------------------------------------

// Set initial gear position by name: up, down
bool IAirVehicle::setSlotInitGearPos(const base::Identifier* const x)
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
double IAirVehicle::getGload() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getGload();
   }
   return value;
}

double IAirVehicle::getMach() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) value = aero->getMach();
   else value = BaseClass::getMach();
   return value;
}

double IAirVehicle::getAngleOfAttack() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getAngleOfAttack();
   }
   return value;
}

double IAirVehicle::getSideSlip() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getSideSlip();
   }
   return value;
}

double IAirVehicle::getFlightPath() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getFlightPath();
   }
   return value;
}

double IAirVehicle::getFuelWt() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getFuelWt();
   }
   return value;
}

double IAirVehicle::getFuelWtMax() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getFuelWtMax();
   }
   return value;
}

double IAirVehicle::getGrossWeight() const
{
   double value {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getGrossWeight();
   }
   return value;
}

double IAirVehicle::getCalibratedAirspeed() const
{
   double value {getTotalVelocityKts()};      // Default: use total velocity
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      value = aero->getCalibratedAirspeed();
   }
   return value;
}

double IAirVehicle::getAmbientPressureRatio() const
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
void IAirVehicle::setTrimSwitchRollInput(const double value)
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
void IAirVehicle::setTrimSwitchPitchInput(const double value)
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
void IAirVehicle::setRudderPedalInput(const double value)
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
void IAirVehicle::setBrakes(const double left, const double right)
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
void IAirVehicle::setFlaps(const double value)
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
double IAirVehicle::getLandingGearPosition() const
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
double IAirVehicle::getWeaponBayDoorPosition() const
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
bool IAirVehicle::isWeightOnWheels() const
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
void IAirVehicle::setGearHandleSwitch(const double value)
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
double IAirVehicle::getSpeedBrakePosition() const
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
double IAirVehicle::getWingSweepAngle() const
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
void IAirVehicle::setSpeedBrakesSwitch(const double value)
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
void IAirVehicle::setWeaponBayDoorSwitch(const double value)
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
void IAirVehicle::setCmdWingSweepAngle(const double value)
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
int IAirVehicle::getNumberOfEngines() const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getNumberOfEngines();
   }
   return n;
}

int IAirVehicle::getEngThrust(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngThrust(data, max);
   }
   return n;
}

int IAirVehicle::getEngThrustIdle(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngThrustIdle(data, max);
   }
   return n;
}

int IAirVehicle::getEngThrustMil(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngThrustMil(data, max);
   }
   return n;
}

int IAirVehicle::getEngThrustAb(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngThrustAb(data, max);
   }
   return n;
}

int IAirVehicle::getEngRPM(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngRPM(data, max);
   }
   return n;
}

int IAirVehicle::getEngN1(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngN1(data, max);
   }
   return n;
}

int IAirVehicle::getEngPLA(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngPLA(data, max);
   }
   return n;
}

int IAirVehicle::getEngFuelFlow(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngFuelFlow(data, max);
   }
   return n;
}

int IAirVehicle::getEngOilPressure(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngOilPressure(data, max);
   }
   return n;
}

int IAirVehicle::getEngInletTemp(double* const data, const int max) const
{
   int n {};
   const IAeroDynamics* aero {getDynamics()};
   if (aero != nullptr) {
      n = aero->getEngInletTemp(data, max);
   }
   return n;
}

int IAirVehicle::getEngNozzle(double* const data, const int max) const
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
bool IAirVehicle::isRadarAltValid() const
{
    double ra {getRadarAltitude()};
    return (ra >= 0 && ra <= 5000);
}

double IAirVehicle::getRadarAltitude() const
{
    return static_cast<double>(getAltitudeAglFt());
}

}
}
