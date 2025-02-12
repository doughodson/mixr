
#include "mixr/instruments/adi/Adi.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/angles.hpp"

#include "mixr/base/util/math_utils.hpp"

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(Adi, "Adi")
EMPTY_DELETEDATA(Adi)

BEGIN_SLOTTABLE(Adi)
    "maxRate",      // rate at which we drive towards pitch and roll
END_SLOTTABLE(Adi)

BEGIN_SLOT_MAP(Adi)
    ON_SLOT(1, setSlotMaxRate, base::IAngle)    // we can be sent an angle (degrees or radians) / per second
    ON_SLOT(1, setSlotMaxRate, base::Number)    // or a number as degrees per second
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Adi)
    // override the instrument event, since we need to use it ourself
    ON_EVENT_OBJ(UPDATE_INSTRUMENTS, onUpdatePitchAdi, base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE, onUpdateRollDegAdi,     base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE2, onUpdateRollRadAdi,    base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE3, onUpdateMaxRateAdi,    base::Number)
END_EVENT_HANDLER()

Adi::Adi()
{
    STANDARD_CONSTRUCTOR()
}

void Adi::copyData(const Adi& org, const bool)
{
    BaseClass::copyData(org);
    scaledPitch = org.scaledPitch;
    pitch = org.pitch;
    roll = org.roll;
    curTheta = org.curTheta;
    curPhi = org.curPhi;
    maxRate = org.maxRate;
}

void Adi::draw()
{
    lcSaveMatrix();
        lcRotate(curPhi);
        lcTranslate(0, scaledPitch);
        BaseClass::draw();
    lcRestoreMatrix();
}

void Adi::updateData(const double dt)
{
    // update our base class first
    BaseClass::updateData(dt);

    // drive our adi toward the actual pitch, from our current pitch, no faster
    // than our MAX_RATE (this allows for greater fidelity, simulates an analog adi)
    {
        const double delta{base::alim (base::angle::aepcdDeg(pitch - curTheta), maxRate * dt)};
        curTheta = base::angle::aepcdDeg(curTheta + delta);
    }

    // now do the same thing for roll
    {
        const double delta{base::alim (base::angle::aepcdRad(roll - curPhi), maxRate * dt)};
        curPhi = base::angle::aepcdRad(curPhi + delta);
    }

    // get our table, and do the linear interpolation ourself
    setInstVal(curTheta);
    scaledPitch = getInstValue();
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotMaxRate(base::Angle)
//------------------------------------------------------------------------------
bool Adi::setSlotMaxRate(const base::IAngle* const x)
{
    bool ok{};
    if (x != nullptr) ok = setMaxRate(x->getValueInDegrees());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMaxRate() -
//------------------------------------------------------------------------------
bool Adi::setSlotMaxRate(const base::Number* const x)
{
    bool ok{};
    if (x != nullptr) ok = setMaxRate(x->asDouble());
    return ok;
}

// Event functions

//------------------------------------------------------------------------------
// onUpdateRollDegAdi() - update roll by degrees
//------------------------------------------------------------------------------
bool Adi::onUpdateRollDegAdi(const base::Number* const newR)
{
    bool ok{};
    if (newR != nullptr) ok = setRollDeg(newR->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateRollRadAdi() - update roll by radians
//------------------------------------------------------------------------------
bool Adi::onUpdateRollRadAdi(const base::Number* const newR)
{
    bool ok{};
    if (newR != nullptr) ok = setRollRad(newR->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdatePitchAdi() - update pitch (degrees)
//------------------------------------------------------------------------------
bool Adi::onUpdatePitchAdi(const base::Number* const newP)
{
    bool ok{};
    if (newP != nullptr) ok = setPitch(newP->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateMaxRateAdi() - set our max rate for the ADI
//------------------------------------------------------------------------------
bool Adi::onUpdateMaxRateAdi(const base::Number* const newMR)
{
    bool ok{};
    if (newMR != nullptr) ok = setMaxRate(newMR->asDouble());
    return ok;
}

// SET functions

//------------------------------------------------------------------------------
// setRollDeg() - set our amount of roll in degrees
//------------------------------------------------------------------------------
bool Adi::setRollDeg(const double newR)
{
    roll = newR * static_cast<double>(base::angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setRollRad() - set roll in radians
//------------------------------------------------------------------------------
bool Adi::setRollRad(const double newR)
{
    roll = newR;
    return true;
}

//------------------------------------------------------------------------------
// setPitch() - set our pitch value (degrees)
//------------------------------------------------------------------------------
bool Adi::setPitch(const double newP)
{
    pitch = newP;
    return true;
}

//------------------------------------------------------------------------------
// setMaxRate() - set our max rate
//------------------------------------------------------------------------------
bool Adi::setMaxRate(const double newMR)
{
    maxRate = newMR;
    return true;
}

}
}
