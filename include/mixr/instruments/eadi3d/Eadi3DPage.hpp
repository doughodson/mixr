
#ifndef __mixr_instruments_Eadi3DPage_HPP__
#define __mixr_instruments_Eadi3DPage_HPP__

#include "mixr/graphics/Page.hpp"
#include "mixr/instruments/eadi3d/Eadi3DObjects.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: Eadi3DPage
//------------------------------------------------------------------------------
class Eadi3DPage : public graphics::Page
{
    DECLARE_SUBCLASS(Eadi3DPage, graphics::Page)

public:
    Eadi3DPage();

    void setAltitude(const double);             // feet
    void setAirspeed(const double);             // knots
    void setHeading(const double);              // deg
    void setAOA(const double);                  // deg
    void setVVI(const double);                  // ft/min
    void setPitch(const double);                // deg
    void setRoll(const double);                 // deg
    void setMach(const double);                 // mach number
    void setGLoad(const double);                // Gs
    void setLandingMode(const bool);            // T = landing mode
    void setPitchSteeringCmd(const double);     // [-1, 1]
    void setRollSteeringCmd(const double);      // [-1, 1]
    void setPitchSteeringValid(const bool);     // T = valid
    void setRollSteeringValid(const bool);      // T = valid
    void setGlideslopeDev(const double);        // dots [-2, 2]
    void setLocalizerDev(const double);         // dots [-2, 2]
    void setTurnRate(const double);             // dots [-2, 2]
    void setSlipInd(const double);              // dots [-2, 2]
    void setGlideslopeValid(const bool);        // T = valid
    void setLocalizerValid(const bool);         // T = valid

    void draw() override;

    bool event(const int event, base::IObject* const obj = nullptr) override;

private:
    // event functions
    bool onEventSetAltitude(const base::INumber* const);
    bool onEventSetAirspeed(const base::INumber* const);
    bool onEventSetHeading(const base::INumber* const);
    bool onEventSetAOA(const base::INumber* const);
    bool onEventSetVVI(const base::INumber* const);
    bool onEventSetPitch(const base::INumber* const);
    bool onEventSetRoll(const base::INumber* const);
    bool onEventSetMach(const base::INumber* const);
    bool onEventSetGLoad(const base::INumber* const);
    bool onEventSetLandingMode(const base::INumber* const);
    bool onEventSetPitchSteeringCmd(const base::INumber* const);
    bool onEventSetRollSteeringCmd(const base::INumber* const);
    bool onEventSetPitchSteeringValid(const base::INumber* const);
    bool onEventSetRollSteeringValid(const base::INumber* const);
    bool onEventSetGlideslopeDev(const base::INumber* const);
    bool onEventSetLocalizerDev(const base::INumber* const);
    bool onEventSetTurnRate(const base::INumber* const);
    bool onEventSetSlipInd(const base::INumber* const);
    bool onEventSetGlideslopeValid(const base::INumber* const);
    bool onEventSetLocalizerValid(const base::INumber* const);

    void globeBall(double pitch, double roll, double pitchCmd, double rollCmd, bool psValid, bool rcValid, bool landMode);
    void scales(double gsDev, double locDev, double turnRate, double slipInd, bool gsValid, bool locValid, bool landMode);
    void windows(double cas, double alt, double aoa, double mach, double vvi, const char* airSpeedType, double gload);
    void heading(double hdg, double hdgCmd);
    void background();

    double radius {};
    Eadi3DObjects eadiObjs;

    double altitudeFT {};
    double airspeedKTS {};
    double headingDEG {};
    double aoaDEG {};
    double vviFPM {};
    double pitchDEG {};
    double rollDEG {};
    double machNo {};

    double Gload {};
    bool   landingMode {};
    double pitchSteeringCmd {};
    double rollSteeringCmd {};
    bool   pitchSteeringValid {};
    bool   rollSteeringValid {};
    double glideslopeDevDOTS {};
    double localizerDevDOTS {};
    double turnRateDOTS {};
    double slipIndDOTS {};
    bool   glideslopeDevValid {};
    bool   localizerDevValid {};
};

}
}

#endif
