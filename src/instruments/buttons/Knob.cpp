
#include "mixr/instruments/buttons/Knob.hpp"
#include "mixr/graphics/Display.hpp"

#include "mixr/base/qty/util/angle_utils.hpp"
#include "mixr/base/relations/Table1.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include <GL/glu.h>
#include <cmath>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(Knob, "Knob")

BEGIN_SLOTTABLE(Knob)
    "valueTable",        // 1) table with rotation and values, else we do 1 to 1
    "endless",           // 2) If true, the knob will continue rotating past last value forever.
    "endlessValueStart", // 3) Value we want the endless knob to start at (no table)
    "endlessValueLimit", // 4) Value we want the endless knob to "reset" at (back to start value - no table)
                         // if you have a value table and have endless set, it will reset at your high limit back to your low limit
END_SLOTTABLE(Knob)

BEGIN_SLOT_MAP(Knob)
    ON_SLOT(1, setSlotValueTable,   base::Table1)
    ON_SLOT(2, setSlotEndless,      base::Boolean)
    ON_SLOT(3, setSlotEndlessStart, base::INumber)
    ON_SLOT(4, setSlotEndlessLimit, base::INumber)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Knob)
    ON_EVENT(ON_MOTION, onMotion)
    ON_EVENT(ON_SINGLE_CLICK, onSingleClick)
END_EVENT_HANDLER()

Knob::Knob()
{
    STANDARD_CONSTRUCTOR()
}

void Knob::copyData(const Knob& org, const bool)
{
    BaseClass::copyData(org);

    setSlotValueTable(org.table);

    value = org.value;
    angle = org.angle;
    endless = org.endless;
    lastAngle = org.lastAngle;
    degsRotation = org.degsRotation;
    findStartTimer = org.findStartTimer;
    startAngle = org.startAngle;
    endlessStart = org.endlessStart;
    endlessLimit = org.endlessLimit;
}

void Knob::deleteData()
{
    setSlotValueTable(nullptr);
}

//------------------------------------------------------------------------------
// setSlotValueTable() - set the table for degrees rotation to value
//------------------------------------------------------------------------------
bool Knob::setSlotValueTable(base::Table1* const x)
{
    if (table != nullptr) table->unref();
    table = nullptr;
    if (x != nullptr) {
        table = x;
        table->ref();
    }
    return true;
}

//------------------------------------------------------------------------------
// setSlotEndless() - set up our table to be endless or not
//------------------------------------------------------------------------------
bool Knob::setSlotEndless(const base::Boolean* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setEndless(x->asBool());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotEndlessStart() - start value of endless knob
//------------------------------------------------------------------------------
bool Knob::setSlotEndlessStart(const base::INumber* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setEndlessStart(x->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotEndlessLimit() - limit value of endless knob
//------------------------------------------------------------------------------
bool Knob::setSlotEndlessLimit(const base::INumber* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setEndlessLimit(x->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// drawFunc() -
//------------------------------------------------------------------------------
void Knob::drawFunc()
{
    BaseClass::drawFunc();

    // check our startX, startY about once every second
    if (findStartTimer > 1) {
        // determine our center position of our graphic
        GLdouble modelMatrix[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
        GLdouble projMatrix[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
        GLint viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);

        // these will hold our new values
        GLdouble winx0 = 0;
        GLdouble winy0 = 0;
        GLdouble winz0 = 0;

        // ok, we have all of our data, send it to be converted to window coordinates
        // we are starting at our matrix position, so our object coordinates are all 0
        gluProject(0, 0, 0, modelMatrix, projMatrix, viewport, &winx0, &winy0, &winz0);
        startX = static_cast<int>(winx0);

        // we have our viewport, so make sure we subract our y from that to get an X,Y from top , left corner instead
        // of bottom left corner
        startY = static_cast<int>(static_cast<GLdouble>(viewport[3]) - winy0);
        findStartTimer = 0;
    }
}

//------------------------------------------------------------------------------
// onMotion() - what to do when our knob it moved
//------------------------------------------------------------------------------
bool Knob::onMotion()
{
    computeRotation();
    if (getDisplay() != nullptr) {
        getDisplay()->buttonEvent(getEventId());
    }
    return true;
}

//------------------------------------------------------------------------------
// onSingleClick() - this tells us we started moving
//------------------------------------------------------------------------------
bool Knob::onSingleClick()
{
    // take over our single click
    start = true;
    return true;
}

//------------------------------------------------------------------------------
// computeRotation() - determine how much we have moved
//------------------------------------------------------------------------------
void Knob::computeRotation()
{
    graphics::Display* myDisplay = (graphics::Display*)findContainerByType(typeid(graphics::Display));
    // compute the rotation amout based on the start X,Y and our current X,Y
    if (myDisplay != nullptr) {

        // get our end position
        int curX = 0, curY = 0;
        myDisplay->getMouse(&curX, &curY);
        int posMoveX = curX - startX;
        int posMoveY = curY - startY;

        // save our last angle
        lastAngle = angle;
        //std::cout << "START X , Y = " << startX << ", " << startY << std::endl;
        //std::cout << "POSITION X, Y = " << posMoveX << ", " << posMoveY << std::endl;
        if (myDisplay->getDisplayOrientation() == graphics::Display::Orientation::CCW90) {
           angle = atan2f(static_cast<float>(posMoveX), static_cast<float>(posMoveY));
        }
        else {
           angle = atan2f(static_cast<float>(posMoveY), static_cast<float>(-posMoveX));
        }

        angle *= static_cast<double>(base::angle::R2DCC);

        if (start) {
            startAngle = static_cast<double>(degsRotation);
            running = 0;
        }
        start = false;

        //std::cout << "START ANGLE = " << startAngle << std::endl;
        //std::cout << "RUNNING ANGLE = " << running << std::endl;

        // now get our difference
        if (lastAngle != angle) {
            double temp = 0;
            if (angle > 0 && lastAngle > 0) {
                if (angle > lastAngle) {
                    temp = angle - lastAngle;
                    running -= temp;
                }
                else {
                    temp = lastAngle - angle;
                    running += temp;
                }
           }
            else if (angle < 0 && lastAngle < 0) {
                if (angle < lastAngle) {
                    temp =  std::fabs(angle - lastAngle);
                    running += temp;
                }
                else {
                    temp = std::fabs(lastAngle - angle);
                    running -= temp;
                }
            }
        }

        if (table != nullptr) {
            double maxX = table->getMaxX();
            double minX = table->getMinX();
            double x = running + startAngle;
            if (x > maxX) x = maxX;
            if (x < minX) x = minX;
            degsRotation = static_cast<int>(x);
            value = table->lfi(x);
            if (endless) {
                double minVal = 0, maxVal = 0;
                table->findMinMax(&minVal, &maxVal);
                if (value == maxVal) {
                    value = minVal;
                    running = 0;
                    degsRotation = 0;
                    startAngle = 0;
                }
            }
        }
        else {
            value = running + startAngle;
            degsRotation = static_cast<int>(value);
            // if we are endless, we have to do some more stuff
            if (endless) {
                // first off, we are just using angle, we don't care about
                if (value >= 0) {
                    if (value > endlessLimit) {
                        startAngle = running;
                        running = 0;
                        value = endlessStart;
                    }
                }
                // don't let our value go below endless start, we are an endless knob in one direction
                else {
                    value = endlessStart;
                    running = 0;
                    startAngle = 0;
                }
            }
        }
    }
}

void Knob::draw()
{
    // rotate our knob!
    lcSaveMatrix();
    if (endless) lcRotate(angle * static_cast<double>(base::angle::D2RCC));
    else lcRotate(-degsRotation * static_cast<double>(base::angle::D2RCC));
    Graphic::draw();
    lcRestoreMatrix();
}

void Knob::updateData(const double dt)
{
    BaseClass::updateData(dt);

    findStartTimer += dt;
}

}
}
