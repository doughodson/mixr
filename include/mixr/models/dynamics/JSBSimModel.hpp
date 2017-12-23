
#ifndef __mixr_models_JSBSimModel_H__
#define __mixr_models_JSBSimModel_H__

#include "mixr/models/dynamics/AerodynamicsModel.hpp"

namespace JSBSim { class FGFDMExec; class FGPropertyManager; }

namespace mixr {
namespace base { class String; class Integer; }
namespace models {

//------------------------------------------------------------------------------
// Class: JSBSimModel
// Description: JSBSim Model
//------------------------------------------------------------------------------
class JSBSimModel final: public AerodynamicsModel
{
    DECLARE_SUBCLASS(JSBSimModel, AerodynamicsModel)

public:
    JSBSimModel();

    double getGload() const final;
    double getMach() const final;
    double getAngleOfAttack() const final;
    double getSideSlip() const final;
    double getFlightPath() const final;
    double getCalibratedAirspeed() const final;
    double getGrossWeight() const final;
    double getFuelWt() const final;
    double getFuelWtMax() const final;
    double getSpeedBrakePosition() const final;
    double getLandingGearPosition() const final;
    bool isWeightOnWheels() const final;
    int getNumberOfEngines() const final;
    int getEngThrust(double* const fn, const int max) const final;
    int getEngRPM(double* const rpm, const int max) const final;
    int getEngFuelFlow(double* const ff, const int max) const final;
    int getEngOilPressure(double* const oil, const int max) const final;    // Pressure: psi
    int getEngInletTemp(double* const tmp, const int max) const final;      // Temp: C
    int getEngNozzle(double* const noz, const int max) const final;         // Nozzle Pos: %
    int getEngPLA(double* const pla, const int max) const final;
    void setControlStickRollInput(const double roll) final;
    void setControlStickPitchInput(const double pitch) final;
    void setTrimSwitchRollInput(const double rollTrim) final;
    void setTrimSwitchPitchInput(const double pitchTrim) final;
    void setGearHandleSwitch(const double sw) final;
    void setSpeedBrakesSwitch(const double sw) final;
    void setBrakes(const double left, const double right) final;
    int setThrottles(const double* const positions, const int num) final;
    void setRudderPedalInput(const double pedal) final;

    void reset() final;

    void dynamics(const double  dt = 0.0) final;

    bool isHeadingHoldOn() const final;
    double getCommandedHeadingD() const final;
    bool setHeadingHoldOn(const bool b) final;
    bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0) final;

    bool isVelocityHoldOn() const final;
    double getCommandedVelocityKts() const final;
    bool setVelocityHoldOn(const bool b) final;
    bool setCommandedVelocityKts(const double v, const double vNps = 0) final;

    bool isAltitudeHoldOn() const final;
    double getCommandedAltitude() const final;
    bool setAltitudeHoldOn(const bool b) final;
    bool setCommandedAltitude(const double a, const double aMps = 0, const double maxPitch = 0) final;

protected:

    JSBSim::FGFDMExec* fdmex{};
    JSBSim::FGPropertyManager* propMgr{};

private:
    const base::String* rootDir{};  // root directory for JSBSim models
    const base::String* model{};    // JSBSim model
    int   debugLevel{};

    double pitchTrimPos{};          // +/- 1.0
    double pitchTrimRate{};         // maxVal(1.0) per sec
    double pitchTrimSw{};
    double rollTrimPos{};           // +/- 1.0
    double rollTrimRate{};          // maxVal(1.0) per sec
    double rollTrimSw{};

    bool   headingHoldOn{};
    bool   altitudeHoldOn{};
    bool   velocityHoldOn{};
    double commandedHeadingDeg{};
    double commandedAltitudeFt{};
    double commandedVelocityKts{};
    bool   hasHeadingHold{};
    bool   hasVelocityHold{};
    bool   hasAltitudeHold{};

private:
   // slot table helper methods
   bool setSlotRootDir(const base::String* const);
   bool setSlotModel(const base::String* const);
   bool setSlotDebugLevel(const base::Integer* const);
};

}
}

#endif
