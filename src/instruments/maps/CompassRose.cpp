
#include "mixr/instruments/maps/CompassRose.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(CompassRose, "CompassRose")
EMPTY_DELETEDATA(CompassRose)

BEGIN_SLOTTABLE(CompassRose)
    "centeredRad",      // our centered radius
    "decenteredRad",    // decentered radius
    "displacement",     // how far to translate
END_SLOTTABLE(CompassRose)       

BEGIN_SLOT_MAP(CompassRose)
    ON_SLOT(1, setSlotCenteredRadius,   base::Number)
    ON_SLOT(2, setSlotDeCenteredRadius, base::Number)
    ON_SLOT(3, setSlotDisplacement,     base::Number)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(CompassRose)
    ON_EVENT_OBJ(UPDATE_VALUE,  onUpdateRotDeg,       base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE2, onUpdateRadius,       base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE3, onUpdateCenRad,       base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE4, onUpdateDecRadius,    base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE5, onUpdateDisplacement, base::Number)
    ON_EVENT_OBJ(UPDATE_VALUE6, onUpdateCentered,     base::Boolean)
END_EVENT_HANDLER()

CompassRose::CompassRose()
{
    STANDARD_CONSTRUCTOR()
    sTicksSD.empty();
    lTicksSD.empty();
    dialSD.empty();
}

void CompassRose::copyData(const CompassRose& org, const bool)
{
    BaseClass::copyData(org);
    
    centered = org.centered;
    cenRadius = org.cenRadius;
    decRadius = org.decRadius;
    displacement = org.displacement;
    sTicksSD.empty();
    lTicksSD.empty();
    dialSD.empty();
    rot = org.rot;
}

// Slot functions
//------------------------------------------------------------------------------
// setSlotCenteredRadius() - sets our radius
//------------------------------------------------------------------------------
bool CompassRose::setSlotCenteredRadius(const base::Number* const newR)
{
    bool ok = false;
    if (newR != nullptr) ok = setCenteredRadius(newR->to_double());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotDeCenteredRadius() - sets our decentered radius
//------------------------------------------------------------------------------
bool CompassRose::setSlotDeCenteredRadius(const base::Number* const newR)
{
    bool ok = false;
    if (newR != nullptr) ok = setDeCenteredRadius(newR->to_double());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotDisplacement() - how far to displace when we decenter
//------------------------------------------------------------------------------
bool CompassRose::setSlotDisplacement(const base::Number* const newD)
{
    bool ok = false;
    if (newD != nullptr) ok = setDisplacement(newD->to_double());
    return ok;
}

// Set functions
//------------------------------------------------------------------------------
// setRotationDeg() - 
//------------------------------------------------------------------------------
bool CompassRose::setRotationDeg(const double newR)
{
    rot = newR * static_cast<double>(base::angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setRotationRad() - 
//------------------------------------------------------------------------------
bool CompassRose::setRotationRad(const double newR)
{
    rot = newR;
    return true;
}

//------------------------------------------------------------------------------
// setCenteredRadius() - 
//------------------------------------------------------------------------------
bool CompassRose::setCenteredRadius(const double newR)
{
    cenRadius = newR;
    return true;
}

//------------------------------------------------------------------------------
// seDeCenteredRadius() - 
//------------------------------------------------------------------------------
bool CompassRose::setDeCenteredRadius(const double newDR)
{
    decRadius = newDR;
    return true; 
}

//------------------------------------------------------------------------------
// setDisplacement() - 
//------------------------------------------------------------------------------
bool CompassRose::setDisplacement(const double newD)
{
    displacement = newD;
    return true; 
}

//------------------------------------------------------------------------------
// setCentered() - 
//------------------------------------------------------------------------------
bool CompassRose::setCentered(const bool newC)
{
    centered = newC;
    return true;
}

// event functions
//------------------------------------------------------------------------------
// onUpdateRotDeg() - update rotation (degrees)
//------------------------------------------------------------------------------
bool CompassRose::onUpdateRotDeg(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setRotationDeg(x->to_double());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateRadius() - change our radius
//------------------------------------------------------------------------------
bool CompassRose::onUpdateRadius(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setRotationRad(x->to_double());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateCenRad() - change our centered radius 
//------------------------------------------------------------------------------
bool CompassRose::onUpdateCenRad(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setCenteredRadius(x->to_double());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateDecRadius() - change our decentered radius
//------------------------------------------------------------------------------
bool CompassRose::onUpdateDecRadius(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setDeCenteredRadius(x->to_double());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateDisplacement() - change our displacement
//------------------------------------------------------------------------------
bool CompassRose::onUpdateDisplacement(const base::Number* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setDisplacement(x->to_double());
    return ok;
}

//------------------------------------------------------------------------------
// onUpdateCentered() - change our centered status
//------------------------------------------------------------------------------
bool CompassRose::onUpdateCentered(const base::Boolean* const x)
{
    bool ok = false; 
    if (x != nullptr) ok = setCentered(x->to_bool());
    return ok;
}


//------------------------------------------------------------------------------
// draw() -- draws the object(s) using the graphic matrix
//------------------------------------------------------------------------------
void CompassRose::draw()
{
    lcSaveMatrix();
        if (!centered) lcTranslate(0, displacement);    
        lcRotate(rot);
        graphics::Graphic::draw();
    lcRestoreMatrix();
}

//------------------------------------------------------------------------------
// updateData() - update our non time-critical stuff here
//------------------------------------------------------------------------------
void CompassRose::updateData(const double dt)
{
    // update baseclass stuff first
    BaseClass::updateData(dt);

    double rad = 0;
    if (centered) rad = cenRadius;
    else rad = decRadius;

    // send our tick marks and dial arc segments the proper data
    send("longticks", UPDATE_VALUE, rad, lTicksSD);
    send("shortticks", UPDATE_VALUE, rad, sTicksSD);
    send("dial", UPDATE_VALUE, rad, dialSD);
}

}
}
