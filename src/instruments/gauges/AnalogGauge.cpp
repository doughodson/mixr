
#include "mixr/instruments/gauges/AnalogGauge.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Number.hpp"
#include <iostream>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(AnalogGauge, "AnalogGauge")
EMPTY_DELETEDATA(AnalogGauge)

BEGIN_SLOTTABLE(AnalogGauge)
    "leftBoundary",                 // the left side of the gauge (inches)
    "rightBoundary",                // the right side of the gauge (inches)
    "outlined",                     // are we an outline or a filled bar?
    "vertical",                     // are we drawing up and down?
END_SLOTTABLE(AnalogGauge)

BEGIN_SLOT_MAP(AnalogGauge)
    ON_SLOT(1, setSlotLeftBoundary,  base::Number)
    ON_SLOT(2, setSlotRightBoundary, base::Number)
    ON_SLOT(3, setSlotIsOutlined,    base::Boolean)
    ON_SLOT(4, setSlotIsVertical,    base::Boolean)
END_SLOT_MAP()

AnalogGauge::AnalogGauge()
{
    STANDARD_CONSTRUCTOR()
}

void AnalogGauge::copyData(const AnalogGauge& org, const bool)
{
    BaseClass::copyData(org);

    gaugePos = org.gaugePos;
    leftBoundary = org.leftBoundary;
    rightBoundary = org.rightBoundary;
    outline = org.outline;
    vertical = org.vertical;
    drawMe = org.drawMe;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void AnalogGauge::drawFunc()
{
    if (drawMe) {
        glPushMatrix();
        // move us just slightly into the background, so things will overlay us
        gaugePos = getInstValue();
        //glTranslatef(0, 0, -0.1f);
        // if we are vertical, draw us growing up and down, else draw us growing left to right
        if (vertical) {
            if (outline) {
                glBegin(GL_LINE_STRIP);
            } else {
                glBegin(GL_POLYGON);
            }
            lcVertex2(leftBoundary, 0);
            lcVertex2(leftBoundary, gaugePos);
            lcVertex2(rightBoundary, gaugePos);
            lcVertex2(rightBoundary, 0);
            glEnd();
        } else {
            if (outline) {
                glBegin(GL_LINE_STRIP);
            } else {
                glBegin(GL_POLYGON);
            }
            lcVertex2(0, leftBoundary);
            lcVertex2(gaugePos, leftBoundary);
            lcVertex2(gaugePos, rightBoundary);
            lcVertex2(0, rightBoundary);
            glEnd();
        }
        glPopMatrix();
    }
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotLeftBoundary() -- sets the left side of the gauge
//------------------------------------------------------------------------------
bool AnalogGauge::setSlotLeftBoundary(const base::Number* const newLB)
{
    bool ok = false;
    if (newLB != nullptr) ok = setLeftBoundary(newLB->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotRightBoundary() -- sets the right side of the gauge
//------------------------------------------------------------------------------
bool AnalogGauge::setSlotRightBoundary(const base::Number* const newRB)
{
    bool ok = false;
    if (newRB != nullptr) ok = setRightBoundary(newRB->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotIsOutlined() -- determines whether we are a filled bar or outlined
//------------------------------------------------------------------------------
bool AnalogGauge::setSlotIsOutlined(const base::Boolean* const newO)
{
    bool ok{};
    if (newO != nullptr) ok = setIsOutlined(newO->asBool());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotIsVertical() - sets our vertical flag
//------------------------------------------------------------------------------
bool AnalogGauge::setSlotIsVertical(const base::Boolean* const newV)
{
    bool ok = false;
    if (newV != nullptr) ok = setIsVertical(newV->asBool());
    return ok;
}

// SET functions
//------------------------------------------------------------------------------
// setLeftBoundary() - left side of gauge
//------------------------------------------------------------------------------
bool AnalogGauge::setLeftBoundary(const double newLB)
{
    leftBoundary = newLB;
    return true;
}

//------------------------------------------------------------------------------
// setRightBoundary() - right side of gauge
//------------------------------------------------------------------------------
bool AnalogGauge::setRightBoundary(const double newRB)
{
    rightBoundary = newRB;
    return true;
}

//------------------------------------------------------------------------------
// setIsOutlined() - set our outline
//------------------------------------------------------------------------------
bool AnalogGauge::setIsOutlined(const bool newO)
{
    outline = newO;
    return true;
}

//------------------------------------------------------------------------------
// setIsVertical() - vertical or horizontal gauge?
//------------------------------------------------------------------------------
bool AnalogGauge::setIsVertical(const bool newV)
{
    vertical = newV;
    return true;
}

//------------------------------------------------------------------------------
// setDrawMe() - determines if we draw our drawFunc or not.
//------------------------------------------------------------------------------
bool AnalogGauge::setDrawMe(const bool x)
{
    drawMe = x;
    return true;
}

}
}
