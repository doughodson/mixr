#include "mixr/instruments/gauges/AoaIndexer.hpp"

#include "mixr/base/PairStream.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/qty/angles.hpp"
#include <GL/glu.h>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(AoAIndexer, "AoAIndexer")

BEGIN_SLOTTABLE(AoAIndexer)
    "aboveMax",     // min value when aoa is over in-range value
    "aboveMin",     // max value when aoa is over in-range value
    "inRangeMax",   // min value when aoa is in range
    "inRangeMin",   // max value when aoa is in range
    "belowMax",     // min value when aoa is below in-range value
    "belowMin",     // max value when aoa is below in-range value
END_SLOTTABLE(AoAIndexer)

BEGIN_SLOT_MAP(AoAIndexer)
    ON_SLOT(1, setSlotAoaRedMax,    base::INumber)
    ON_SLOT(2, setSlotAoaRedMin,    base::INumber)
    ON_SLOT(3, setSlotAoaGreenMax,  base::INumber)
    ON_SLOT(4, setSlotAoaGreenMin,  base::INumber)
    ON_SLOT(5, setSlotAoaYellowMax, base::INumber)
    ON_SLOT(6, setSlotAoaYellowMin, base::INumber)
END_SLOT_MAP()

AoAIndexer::AoAIndexer()
{
    STANDARD_CONSTRUCTOR()
    selectSD.empty();
}

void AoAIndexer::copyData(const AoAIndexer& org, const bool)
{
    BaseClass::copyData(org);

    aoaState = org.aoaState;
    displayList = org.displayList;
    isDlist = org.isDlist;
    aoaRedMax = org.aoaRedMax;
    aoaRedMin = org.aoaRedMin;
    aoaYellowMax = org.aoaYellowMax;
    aoaYellowMin = org.aoaYellowMin;
    aoaGreenMax = org.aoaGreenMax;
    aoaGreenMin = org.aoaGreenMin;
    selectSD.empty();
}

void AoAIndexer::deleteData()
{
    // Delete display lists
    if (displayList > 0) {
        glDeleteLists(displayList,1);
    }
    isDlist = false;
}

//------------------------------------------------------------------------------
// drawFunc()
//------------------------------------------------------------------------------
void AoAIndexer::drawFunc()
{
    // if we have components, we are going to check for a ROTARY
    base::PairStream* subcomponents = getComponents();
    if (subcomponents != nullptr) {
      subcomponents->unref();
      subcomponents = nullptr;
      return;
    }

    GLfloat currentColor[4];
    GLfloat lw = 0;
    glGetFloatv(GL_CURRENT_COLOR, &currentColor[0]);
    glGetFloatv(GL_LINE_WIDTH, &lw);

    // check our state and draw accordingly
    if (aoaState == 1) {
        glPushMatrix();
            glColor3f(1, 0, 0);
            glTranslatef(0, 1.0, 0);
            glLineWidth(3.0);
            glBegin(GL_LINES);
                glVertex2f(-0.05f, 0.0);
                glVertex2f(-0.2f, 0.4f);
                glVertex2f(0.05f, 0.0);
                glVertex2f(0.2f, 0.4f);
            glEnd();
        glPopMatrix();
    }
    if (aoaState == -1) {
        glPushMatrix();
            glColor3f(1, 1, 0);
            glLineWidth(3.0);
            glTranslatef(0, -1.0, 0);
            glBegin(GL_LINES);
                glVertex2f(-0.05f, 0.0);
                glVertex2f(-0.2f, -0.4f);
                glVertex2f(0.05f, 0.0);
                glVertex2f(0.2f, -0.4f);
            glEnd();
        glPopMatrix();
    }
    if (aoaState == 0) {
        // now draw the lines to blend out the semi-circle
        glLineWidth(3.0);
        glPushMatrix();
            glColor3f(0,0,0);
            glBegin(GL_LINES);
                glVertex2f(-0.25f, 0.025f);
                glVertex2f(0.25f, 0.025f);
                glVertex2f(-0.25f, -0.025f);
                glVertex2f(0.25f, -0.025f);
            glEnd();
        glPopMatrix();

        // we draw two semicircles that are almost touching
        glPushMatrix();
            glColor3f(0, 1, 0);
            glTranslatef(0, 0.025f, 0);
            GLUquadricObj * qobj = gluNewQuadric();
            gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
            gluPartialDisk(qobj, 0, 0.2, 1000, 1, 270, 180);
            gluDeleteQuadric(qobj);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0, 1, 0);
            glTranslatef(0, -0.025f, 0);
            glRotatef(180, 0, 0, 1);
            GLUquadricObj * qobj2 = gluNewQuadric();
            gluQuadricDrawStyle(qobj2, GLU_SILHOUETTE);
            gluPartialDisk(qobj2, 0, 0.2, 1000, 1, 270, 180);
            gluDeleteQuadric(qobj2);
        glPopMatrix();
    }

    // if we haven't created our display list, do it now
    if (!isDlist) {
        displayList = glGenLists(1);
        glNewList(displayList,GL_COMPILE);
        // finally draw our outlines
        // top outline
        glPushMatrix();
            glColor3f(0.3f, 0.3f, 0.3f);
            glTranslatef(0, 1.0, 0);
            glLineWidth(3.0);
            glBegin(GL_LINES);
                glVertex2f(-0.05f, 0.0);
                glVertex2f(-0.2f, 0.4f);
                glVertex2f(0.05f, 0.0);
                glVertex2f(0.2f, 0.4f);
            glEnd();
        glPopMatrix();
        // circle outlines
        // now draw the lines to blend out the semi-circle
        glLineWidth(3.0);
        glPushMatrix();
            glColor3f(0,0,0);
            glBegin(GL_LINES);
                glVertex2f(-0.25f, 0.025f);
                glVertex2f(0.25f, 0.025f);
                glVertex2f(-0.25f, -0.025f);
                glVertex2f(0.25f, -0.025f);
            glEnd();
        glPopMatrix();

        // we draw two semicircles that are almost touching
        glPushMatrix();
            glColor3f(0.3f, 0.3f, 0.3f);
            glTranslatef(0, 0.025f, 0);
            GLUquadricObj * qobj3 = gluNewQuadric();
            gluQuadricDrawStyle(qobj3, GLU_SILHOUETTE);
            gluPartialDisk(qobj3, 0, 0.2, 1000, 1, 270, 180);
            gluDeleteQuadric(qobj3);
        glPopMatrix();
        glPushMatrix();
            glColor3f(0.3f, 0.3f, 0.3f);
            glTranslatef(0, -0.025f, 0);
            glRotatef(180, 0, 0, 1);
            GLUquadricObj * qobj4 = gluNewQuadric();
            gluQuadricDrawStyle(qobj4, GLU_SILHOUETTE);
            gluPartialDisk(qobj4, 0, 0.2, 1000, 1, 270, 180);
            gluDeleteQuadric(qobj4);
        glPopMatrix();

        // draw the bottom outline
        glPushMatrix();
            glColor3f(0.3f, 0.3f, 0.3f);
            glLineWidth(3.0);
            glTranslatef(0, -1.0, 0);
            glBegin(GL_LINES);
                glVertex2f(-0.05f, 0.0);
                glVertex2f(-0.2f, -0.4f);
                glVertex2f(0.05f, 0.0);
                glVertex2f(0.2f, -0.4f);
            glEnd();
            glPopMatrix();
            // end our display list
            glEndList();
            // set our flag
            isDlist = true;
        }
        // if we already have the display list, then call it
    else if (displayList > 0) glCallList(displayList);

    glColor4fv(currentColor);
    glLineWidth(lw);
}


//------------------------------------------------------------------------------
// updateData() -- updates non-critical time threads
//------------------------------------------------------------------------------
void AoAIndexer::updateData(const double dt)
{
    // update our baseclass
    BaseClass::updateData(dt);

    double aoa = getInstValue();

    // let's get our values
    //std::cout << "AOA RED MAX = " << aoaRedMax << std::endl;
    //std::cout << "AOA RED MIN = " << aoaRedMin << std::endl;
    //std::cout << "AOA YELLOW MAX = " << aoaYellowMax << std::endl;
    //std::cout << "AOA YELLOW MIN = " << aoaYellowMin << std::endl;
    //std::cout << "AOA GREEN MAX = " << aoaGreenMax << std::endl;
    //std::cout << "AOA GREEN MIN = " << aoaGreenMin << std::endl;

    // positive (red state)
    if (aoa >= aoaRedMin && aoa < aoaRedMax) aoaState = 1;
    // neutral (green state)
    else if (aoa >= aoaGreenMin && aoa < aoaGreenMax) aoaState = 0;
    // negative (yellow state)
    else if (aoa >= aoaYellowMin && aoa < aoaYellowMax) aoaState = -1;
    else aoaState = 2;

    int x = 1;
    // map these values into the right indexer value
    if (aoaState == 0) x = 2;
    else if (aoaState == 1) x = 4;
    else if (aoaState == -1) x = 3;

    // send our select down
    send("index", SELECT, x, selectSD);
}

// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotAoaRedMax()
//------------------------------------------------------------------------------
bool AoAIndexer::setSlotAoaRedMax(const base::INumber* const newRMax)
{
    bool ok = false;
    if (newRMax != nullptr) ok = setAoaRedMax(newRMax->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotAoaRedMin()
//------------------------------------------------------------------------------
bool AoAIndexer::setSlotAoaRedMin(const base::INumber* const newRMin)
{
    bool ok = false;
    if (newRMin != nullptr) ok = setAoaRedMin(newRMin->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotAoaYellowMax()
//------------------------------------------------------------------------------
bool AoAIndexer::setSlotAoaYellowMax(const base::INumber* const newYMax)
{
    bool ok = false;
    if (newYMax != nullptr) ok = setAoaYellowMax(newYMax->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotAoaYellowMin()
//------------------------------------------------------------------------------
bool AoAIndexer::setSlotAoaYellowMin(const base::INumber* const newYMin)
{
    bool ok = false;
    if (newYMin != nullptr) ok = setAoaYellowMin(newYMin->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotAoaGreenMax()
//------------------------------------------------------------------------------
bool AoAIndexer::setSlotAoaGreenMax(const base::INumber* const newGMax)
{
    bool ok = false;
    if (newGMax != nullptr) ok = setAoaGreenMax(newGMax->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotAoaGreenMin()
//------------------------------------------------------------------------------
bool AoAIndexer::setSlotAoaGreenMin(const base::INumber* const newGMin)
{
    bool ok = false;
    if (newGMin != nullptr) ok = setAoaGreenMin(newGMin->asDouble());
    return ok;
}

// SET functions
//------------------------------------------------------------------------------
// setAoaRedMin() - set min "red" value
//------------------------------------------------------------------------------
bool AoAIndexer::setAoaRedMin(const double a)
{
    aoaRedMin = a;
    return true;
}

//------------------------------------------------------------------------------
// setAoaRedMax() - set the max "red" value
//------------------------------------------------------------------------------
bool AoAIndexer::setAoaRedMax(const double b)
{
    aoaRedMax = b;
    return true;
}

//------------------------------------------------------------------------------
// setAoaYellowMin() - set "yellow" minimum
//------------------------------------------------------------------------------
bool AoAIndexer::setAoaYellowMin(const double a)
{
    aoaYellowMin = a;
    return true;
}

//------------------------------------------------------------------------------
// setAoaYellowMax() - set "yellow" max
//------------------------------------------------------------------------------
bool AoAIndexer::setAoaYellowMax(const double b)
{
    aoaYellowMax = b;
    return true;
}

//------------------------------------------------------------------------------
// setAoaGreenMin() - set "green" min
//------------------------------------------------------------------------------
bool AoAIndexer::setAoaGreenMin(const double a)
{
    aoaGreenMin = a;
    return true;
}

//------------------------------------------------------------------------------
// setAoaGreenMax() - set "green" max
//------------------------------------------------------------------------------
bool AoAIndexer::setAoaGreenMax(const double b)
{
    aoaGreenMax = b;
    return true;
}

}
}
