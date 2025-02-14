
#include "mixr/instruments/dials/AnalogDial.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include <GL/glu.h>
#include <iostream>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(AnalogDial, "AnalogDial")
EMPTY_DELETEDATA(AnalogDial)

BEGIN_EVENT_HANDLER(AnalogDial)
    ON_EVENT_OBJ(UPDATE_VALUE, onUpdateRadius, base::INumber)
END_EVENT_HANDLER()

BEGIN_SLOTTABLE(AnalogDial)
    "startAngle",           // 1) angle in which we start drawing our background from
    "sweepAngle",           // 2) sweep angle (if not drawing a full circle and not drawing a semi-circle, you can determine how far you want the outline to draw)
    "dialRadius",           // 3) how far do we want our background to extend?
    "mobile",               // 4) do we rotate if given a value, or do we just pass it down to our components?
    "slices",               // 5) number of slices to use
END_SLOTTABLE(AnalogDial)

BEGIN_SLOT_MAP(AnalogDial)
    ON_SLOT(1, setSlotOriginAngle, base::INumber)
    ON_SLOT(2, setSlotSweepAngle,  base::INumber)
    ON_SLOT(3, setSlotRadius,      base::INumber)
    ON_SLOT(4, setSlotMobile,      base::Boolean)
    ON_SLOT(5, setSlotSlices,      base::Integer)
END_SLOT_MAP()

AnalogDial::AnalogDial()
{
    STANDARD_CONSTRUCTOR()
}

void AnalogDial::copyData(const AnalogDial& org, const bool)
{
    BaseClass::copyData(org);

    originAngle = org.originAngle;
    sweepAngle = org.sweepAngle;
    radius = org.radius;
    positionAngle = org.positionAngle;
    isMobile = org.isMobile;
    slices = org.slices;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void AnalogDial::drawFunc()
{
    GLfloat ocolor[4];
    glGetFloatv(GL_CURRENT_COLOR, ocolor);

    // get our position from our base instrument class, which handles scaling and such
    positionAngle = getInstValue();

    glPushMatrix();
        // move us just slightly into the background, so things will overlay us
        glTranslatef(0, 0, -0.1f);
        if (isMobile) glRotatef(static_cast<GLfloat>(-positionAngle), 0, 0, 1);
        // draw our background
        GLUquadricObj *qobj = gluNewQuadric();
        gluQuadricDrawStyle(qobj, GLU_FILL);
        // if we are a semi-circle, we ignore our start and sweep angles, and draw a semicircle
        gluPartialDisk(qobj, 0.0,  radius, slices, 1, originAngle, sweepAngle);
        gluDeleteQuadric(qobj);
    glPopMatrix();

    glColor4fv(ocolor);
}

//------------------------------------------------------------------------------
// onUpdateRadius() - adjust our radius
//------------------------------------------------------------------------------
bool AnalogDial::onUpdateRadius(const base::INumber* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setRadius(x->asDouble());
    return ok;
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotOriginAngle() --
//------------------------------------------------------------------------------
bool AnalogDial::setSlotOriginAngle(const base::INumber* newAngle)
{
    bool ok = false;
    if (newAngle != nullptr) {
        ok = setOriginAngle(newAngle->asDouble());
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotSweepAngle() --
//------------------------------------------------------------------------------
bool AnalogDial::setSlotSweepAngle(const base::INumber* newSweepAngle)
{
    bool ok = false;
    if (newSweepAngle != nullptr) ok = setSweepAngle(newSweepAngle->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotradius() -- sets our background radius of the "dial"
//------------------------------------------------------------------------------
bool AnalogDial::setSlotRadius(const base::INumber* newR)
{
    bool ok = false;
    if (newR != nullptr) ok = setRadius(newR->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotMobile() -- allows us to rotate around our dial
//------------------------------------------------------------------------------
bool AnalogDial::setSlotMobile(const base::Boolean* newM)
{
    bool ok = false;
    if (newM != nullptr) ok = setMobile(newM->asBool());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotSlices() -- how many slices to use?
//------------------------------------------------------------------------------
bool AnalogDial::setSlotSlices(const base::Integer* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setSlices(x->asInt());
    return ok;
}

// SET FUNCTIONS
//------------------------------------------------------------------------------
// setOriginAngle() - start angle
//------------------------------------------------------------------------------
bool AnalogDial::setOriginAngle(const double na)
{
    originAngle = na;
    return true;
}

//------------------------------------------------------------------------------
// setSweepAngle() - how far to sweep (degrees)
//------------------------------------------------------------------------------
bool AnalogDial::setSweepAngle(const double newSA)
{
    sweepAngle = newSA;
    return true;
}

//------------------------------------------------------------------------------
// setRadius() - set the radius of our dial
//------------------------------------------------------------------------------
bool AnalogDial::setRadius(const double newR)
{
    radius = newR;
    return true;
}

//------------------------------------------------------------------------------
// setMobile() - should the whole dial move, or just certain parts?
//------------------------------------------------------------------------------
bool AnalogDial::setMobile(const bool newM)
{
    isMobile = newM;
    return true;
}

//------------------------------------------------------------------------------
// setSlices() - determines how "smooth" our dial looks
//------------------------------------------------------------------------------
bool AnalogDial::setSlices(const int x)
{
    slices = x;
    return true;
}

}
}
