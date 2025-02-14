
#include "mixr/instruments/landinggear/LandingGear.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include <iostream>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(LandingGear, "LandingGear")
EMPTY_DELETEDATA(LandingGear)

BEGIN_SLOTTABLE(LandingGear)
    "gearDownValue",  // the gear position that defines the "down" state of the gear handle
    "gearUpValue",    // gear position that defines the "up" state of the gear handle
    // Note: Everything in between these values are considered "in Transit" and will be
    // treated as such
END_SLOTTABLE(LandingGear)

BEGIN_SLOT_MAP(LandingGear)
    ON_SLOT(1, setSlotGearDownValue, base::INumber)
    ON_SLOT(2, setSlotGearUpValue,   base::INumber)
END_SLOT_MAP()

LandingGear::LandingGear()
{
    STANDARD_CONSTRUCTOR()
    gearSelSD.empty();
}

void LandingGear::copyData(const LandingGear& org, const bool)
{
    BaseClass::copyData(org);
    gearState = org.gearState;
    inTransit = org.inTransit;
    gearPos = org.gearPos;
    gearDV = org.gearDV;
    gearUV = org.gearUV;
    gearSelSD.empty();
    haveRotary = org.haveRotary;
}

// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotGearDownValue() -
//------------------------------------------------------------------------------
bool LandingGear::setSlotGearDownValue(const base::INumber* const newDV)
{
    bool ok{};
    if (newDV != nullptr) ok = setGearDownValue(newDV->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotGearUpValue() -
//------------------------------------------------------------------------------
bool LandingGear::setSlotGearUpValue(const base::INumber* const newUV)
{
    bool ok{};
    if (newUV != nullptr) ok = setGearUpValue(newUV->asDouble());
    return ok;
}

// SET functions
//------------------------------------------------------------------------------
// setGearDownValue() - set the value at which the gear goes down
//------------------------------------------------------------------------------
bool LandingGear::setGearDownValue(const double newDV)
{
    gearDV = newDV;
    return true;
}

//------------------------------------------------------------------------------
// setGearUpValue() -
//------------------------------------------------------------------------------
bool LandingGear::setGearUpValue(const double newUV)
{
    gearUV = newUV;
    return true;
}

//------------------------------------------------------------------------------
// updateData() - update non time-critical stuff here
//------------------------------------------------------------------------------
void LandingGear::updateData(const double dt)
{
    BaseClass::updateData(dt);

    // this will store our last value, so we know which way we are going
    double lastPos{gearPos};
    gearPos = getInstValue();

    if (gearPos == gearUV) {
        gearState = 0;
        inTransit = false;
    } else if (gearPos == gearDV) {
        gearState = 1;
        inTransit = false;
    }
    // if we aren't equal to either, we are in transit
    else {
        inTransit = true;
        // we are going towards the gear up value
        if (gearPos < lastPos) gearState = 0;
        // we are going towards the down value
        else if (gearPos > lastPos) gearState = 1;
        // if we are equal, we do nothing
    }

    // now send our select down based on our transition flag and gear pos
    int x{};
    if (gearState == 0 && !inTransit) x = 1;
    else if (gearState == 0 && inTransit) x = 2;
    else if (gearState == 1 && inTransit) x = 3;
    else if (gearState == 1 && !inTransit) x = 4;

    send("gearpos", SELECT, x, gearSelSD);

    // determine if we have a rotary
    base::Pair* pair{static_cast<base::Pair*>(findByName("gearpos"))};
    if (pair != nullptr) haveRotary = true;
}

//------------------------------------------------------------------------------
// drawFunc()
//------------------------------------------------------------------------------
void LandingGear::drawFunc()
{
    // only do this if we don't have a rotary components
    if (haveRotary) return;

    GLfloat currentColor[4];
    GLfloat lw = 0;
    glGetFloatv(GL_CURRENT_COLOR, &currentColor[0]);
    glGetFloatv(GL_LINE_WIDTH, &lw);

    // draw the landing gear dependent on the gear state
    // gear is up
    if (gearState == 0) {
        glPushMatrix();
            glTranslatef(0, 1, 0);
            // the lines on the handle
            glColor3f(0,0,0);
            glBegin(GL_LINES);
                glVertex2f(-0.3f, -0.6f);
                glVertex2f(-0.3f, 0.6f);
                glVertex2f(-0.1f, -0.6f);
                glVertex2f(-0.1f, 0.6f);
                glVertex2f(0.1f, -0.6f);
                glVertex2f(0.1f, 0.6f);
                glVertex2f(0.3f, -0.6f);
                glVertex2f(0.3f, 0.6f);
            glEnd();
            if (inTransit) glColor3f(1, 0, 0);
            else glColor3f(0.9f, 0.9f, 0.9f);
            // the handle
            glBegin(GL_POLYGON);
                glVertex2f(-0.4f, -0.6f);
                glVertex2f(0.4f, -0.6f);
                glVertex2f(0.4f, 0.6f);
                glVertex2f(-0.4f, 0.6f);
            glEnd();
        glPopMatrix();
        // the arm
        glPushMatrix();
            glColor3f(0.3f, 0.3f, 0.3f);
            glBegin(GL_POLYGON);
                glVertex2f(-0.3f, 0);
                glVertex2f(0.3f, 0);
                glVertex2f(0.3f, 1);
                glVertex2f(-0.3f, 1);
            glEnd();
        glPopMatrix();
    }
    // gear is down
    if (gearState == 1) {
        glPushMatrix();
            glTranslatef(0, -1, 0);
            // the lines on the handle
            glColor3f(0,0,0);
            glBegin(GL_LINES);
                glVertex2f(-0.3f, -0.6f);
                glVertex2f(-0.3f, 0.6f);
                glVertex2f(-0.1f, -0.6f);
                glVertex2f(-0.1f, 0.6f);
                glVertex2f(0.1f, -0.6f);
                glVertex2f(0.1f, 0.6f);
                glVertex2f(0.3f, -0.6f);
                glVertex2f(0.3f, 0.6f);
            glEnd();
            if (inTransit) glColor3f(1, 0, 0);
            else glColor3f(0.9f, 0.9f, 0.9f);
            // the handle
            glBegin(GL_POLYGON);
                glVertex2f(-0.4f, -0.6f);
                glVertex2f(0.4f, -0.6f);
                glVertex2f(0.4f, 0.6f);
                glVertex2f(-0.4f, 0.6f);
            glEnd();
        glPopMatrix();
        // the arm
        glPushMatrix();
            glColor3f(0.3f, 0.3f, 0.3f);
            glBegin(GL_POLYGON);
                glVertex2f(-0.3f, 0);
                glVertex2f(0.3f, 0);
                glVertex2f(0.3f, -1);
                glVertex2f(-0.3f, -1);
            glEnd();
        glPopMatrix();
    }
    glColor4fv(currentColor);
    glLineWidth(lw);
}

}
}
