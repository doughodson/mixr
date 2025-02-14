
#include "mixr/instruments/landinggear/LandingLight.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include <GL/glu.h>
#include <iostream>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(LandingLight, "LandingLight")
EMPTY_DELETEDATA(LandingLight)

BEGIN_SLOTTABLE(LandingLight)
    "lightRadius",          // 3 Radius of our light (if not components)
END_SLOTTABLE(LandingLight)

BEGIN_SLOT_MAP(LandingLight)
    ON_SLOT(1, setSlotLightRadius, base::INumber)
END_SLOT_MAP()

LandingLight::LandingLight()
{
    STANDARD_CONSTRUCTOR()
    selSD.empty();
}

void LandingLight::copyData(const LandingLight& org, const bool)
{
    BaseClass::copyData(org);

    gearCurrent = org.gearCurrent;
    lRadius = org.lRadius;
    selSD.empty();
}

// set functions
//------------------------------------------------------------------------------
// setLightRadius() - how big is our light?
//------------------------------------------------------------------------------
bool LandingLight::setLightRadius(const double newLR)
{
    lRadius = newLR;
    return true;
}


//------------------------------------------------------------------------------
// setSlotLightRadius() - sets our light radius to be drawn
//------------------------------------------------------------------------------
bool LandingLight::setSlotLightRadius(const base::INumber* const newLR)
{
    bool ok{};
    if (newLR != nullptr) ok = setLightRadius(newLR->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// drawFunc()
//------------------------------------------------------------------------------
void LandingLight::drawFunc()
{
    // if the user specifies a light radius, then it will draw this way,
    if (lRadius == 0) return;
    GLfloat currentColor[4]{};
    GLfloat lw{};
    glGetFloatv(GL_CURRENT_COLOR, &currentColor[0]);
    glGetFloatv(GL_LINE_WIDTH, &lw);

    // all we need is the gear up value, and we can toggle accordingly
    double gearUpVal{getGearUpValue()};
    double gearDownVal{getGearDownValue()};
    double lastC{gearCurrent};
    gearCurrent = getInstValue();

    glPushMatrix();
        // determine which way we are going
        if (lastC >= gearCurrent) {
            // going towards the up position (getting closer to 0)
            if (gearCurrent <= gearUpVal) glColor3f(0, 0, 0);
            else glColor3f(0, 1, 0);
        } else if (lastC < gearCurrent) {
            // going towards the down position (getting closer to 1)
            if (gearCurrent >= gearDownVal) glColor3f(0, 1, 0);
            else glColor3f(0, 0, 0);
        }

        GLUquadricObj *qobj{gluNewQuadric()};
        gluDisk(qobj, 0,  lRadius, 1000, 1);
        gluDeleteQuadric(qobj);
    glPopMatrix();

    glColor4fv(currentColor);
    glLineWidth(lw);
}

//------------------------------------------------------------------------------
// updateData() -
//------------------------------------------------------------------------------
void LandingLight::updateData(const double dt)
{
    BaseClass::updateData(dt);

    double gearUpVal{getGearUpValue()};
    double gearDownVal{getGearDownValue()};
    double lastC{gearCurrent};
    gearCurrent = getInstValue();

    bool upDownFlag{};
    // determine which way we are going
    if (lastC >= gearCurrent) {
        // going towards the up position (getting closer to 0)
        if (gearCurrent <= gearUpVal) upDownFlag = false;
        else upDownFlag = true;
    } else if (lastC < gearCurrent) {
        // going towards the down position (getting closer to 1)
        if (gearCurrent >= gearDownVal) upDownFlag = true;
        else upDownFlag = false;
    }

    send("index", SELECT, upDownFlag, selSD);
}

}
}
