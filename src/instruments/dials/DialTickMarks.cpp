
#include "mixr/instruments/dials/DialTickMarks.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include <iostream>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(DialTickMarks, "DialTickMarks")

BEGIN_SLOTTABLE(DialTickMarks)
    "length",           // length of the tick marks
    "quantity",         // tells us how many to draw, default is 1
    "tickGraphic",      // sets a graphic to use instead of a line
END_SLOTTABLE(DialTickMarks)

BEGIN_SLOT_MAP(DialTickMarks)
    ON_SLOT(1, setSlotLength,       base::INumber)
    ON_SLOT(2, setSlotQuantity,     base::Integer)
    ON_SLOT(3, setSlotTickGraphic,  graphics::Graphic)
END_SLOT_MAP()

DialTickMarks::DialTickMarks()
{
    STANDARD_CONSTRUCTOR()
}

void DialTickMarks::copyData(const DialTickMarks& org, const bool)
{
    BaseClass::copyData(org);

    length = org.length;
    quantity = org.quantity;
        setTickGraphic(org.myGraphic);
    }

void DialTickMarks::deleteData()
{
    if (myGraphic != nullptr) myGraphic->unref();
    myGraphic = nullptr;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void DialTickMarks::drawFunc()
{
    GLfloat currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);

    // get our data from our baseclass (AnalogDial)
    double sweepAngle{getSweepAngle()};
    double startAngle{getStartAngle()};
    double tRadius{getRadius()};

    // figure our rotation angle per tick mark (deg)
    double rotation{sweepAngle/quantity};

    glPushMatrix();
        // rotate to our start angle
        glRotatef(static_cast<GLfloat>(-startAngle), 0, 0, 1);
        for (int i = 0; i < quantity + 1; i++) {
            // if we have a graphic, draw that, else we draw a line
            if (myGraphic == nullptr) {
                glBegin(GL_LINES);
                    lcVertex2(0, tRadius);
                    lcVertex2(0, tRadius - length);
                glEnd();
            }
            else {
                myGraphic->lcSaveMatrix();
                    myGraphic->lcTranslate(0, tRadius);
                    myGraphic->draw();
                myGraphic->lcRestoreMatrix();
            }
            glRotatef(static_cast<GLfloat>(-rotation), 0, 0, 1);
        }
    glPopMatrix();

    glColor4fv(currentColor);
}

// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotLength() -- sets our tick mark length
//------------------------------------------------------------------------------
bool DialTickMarks::setSlotLength(const base::INumber* const newLength)
{
    bool ok = true;
    if (newLength != nullptr) ok = setLength(newLength->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotQuantity() -- sets the number of tick marks
//------------------------------------------------------------------------------
bool DialTickMarks::setSlotQuantity(const base::Integer* const newQ)
{
    bool ok = true;
    if (newQ != nullptr) ok = setQuantity(newQ->asInt());
    return ok;
}

bool DialTickMarks::setSlotTickGraphic(const graphics::Graphic* const graphic)
{
    return setTickGraphic(graphic);
}

// Set function
//------------------------------------------------------------------------------
// setTickGraphic() -- sets our graphic for using as tick marks
//------------------------------------------------------------------------------
bool DialTickMarks::setTickGraphic(const graphics::Graphic* const newGraphic)
{
    bool ok = true;
    if (myGraphic != nullptr) {
        myGraphic->unref();
        myGraphic = nullptr;
    }

    if (newGraphic != nullptr) {
        myGraphic = newGraphic->clone();
        myGraphic->container(this);
        ok = true;
    }
    return ok;
}
//------------------------------------------------------------------------------
// setLength() -- sets tick mark length
//------------------------------------------------------------------------------
bool DialTickMarks::setLength(const double newLength)
{
    length = newLength;
    return true;
}
//------------------------------------------------------------------------------
// setQuantity() -- sets the number of tick marks
//------------------------------------------------------------------------------
bool DialTickMarks::setQuantity(const int newQ)
{
    quantity = newQ;
    return true;
}

}
}
