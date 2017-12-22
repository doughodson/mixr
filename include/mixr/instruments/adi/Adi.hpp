
#ifndef __mixr_instruments_Adi_H__
#define __mixr_instruments_Adi_H__

#include "mixr/instruments/Instrument.hpp"

#include "mixr/base/units/angle_utils.hpp"

namespace mixr {
namespace base { class Number; class Angle; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: Adi
//
// Description: Adi that knows how to translate and rotate its pitch and roll
// ladder according to aircraft pitch and roll.  You can either push data
// down by using the send function (see below) or you can get a pointer and
// use the member functions setPitch() and setRoll().
//
// Inputs for the send command:
//      UPDATE_INSTRUMENTS = updates pitch (this overrides the instrument function, because
//      we want to drive the ADI towards pitch, not have real time pitch, always in degrees)
//      UPDATE_VALUE = updates roll (degrees)
//      UPDATE_VALUE2 = updates roll (radians)
//      UPDATE_VALUE3 = updates max rate (degrees/second)
//------------------------------------------------------------------------------
class Adi : public Instrument
{
    DECLARE_SUBCLASS(Adi, Instrument)

public:
    Adi();

    double getPitchInches() const       { return scaledPitch; }
    double getPitch() const             { return getPreScaleInstValue(); }
    double getRollDeg() const           { return curPhi * static_cast<double>(base::angle::R2DCC); }
    double getRollRad() const           { return curPhi; }
    double getMaxRate() const           { return maxRate; }

    virtual bool setRollRad(const double);
    virtual bool setRollDeg(const double);
    virtual bool setPitch(const double);
    virtual bool setMaxRate(const double);

    void draw() override;

    bool event(const int event, base::Object* const obj = nullptr) override;
    void updateData(const double dt = 0) override;

private:
    // event function
    bool onUpdatePitchAdi(const base::Number* const);
    bool onUpdateRollDegAdi(const base::Number* const);
    bool onUpdateRollRadAdi(const base::Number* const);
    bool onUpdateMaxRateAdi(const base::Number* const);

    double pitch {};         // actual pitch (degrees)
    double scaledPitch {};   // our pitch value (inches)
    double curTheta {};      // our current pitch value (degrees)
    double curPhi {};        // our current roll value (radians)
    double roll {};          // our roll value (radians)
    double maxRate {500.0};  // maximum mechanical rate at which the adi can move pitch or roll
                             // default set high degrees/second (for instantaneous movement)

private:
    // slot table helper methods
    bool setSlotMaxRate(const base::Angle* const);
    bool setSlotMaxRate(const base::Number* const);
};

}
}

#endif
