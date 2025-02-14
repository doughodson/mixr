
#include "mixr/instruments/dials/DialArcSegment.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include <iostream>

#include <GL/glu.h>
#include <cmath>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(DialArcSegment, "DialArcSegment")
EMPTY_DELETEDATA(DialArcSegment)

BEGIN_SLOTTABLE(DialArcSegment)
    "dynamic",                  // do we draw around a circle according to our value?
    "outerRadius",              // this will allow us to draw nice clean arcs
    "filled",                   // fill the inside?
END_SLOTTABLE(DialArcSegment)

BEGIN_SLOT_MAP(DialArcSegment)
    ON_SLOT(1, setSlotIsDynamic,   base::Boolean)
    ON_SLOT(2, setSlotOuterRadius, base::INumber)
    ON_SLOT(3, setSlotFilled,      base::Boolean)
END_SLOT_MAP()

DialArcSegment::DialArcSegment()
{
    STANDARD_CONSTRUCTOR()
}

void DialArcSegment::copyData(const DialArcSegment& org, const bool)
{
    // copy base stuff first
    BaseClass::copyData(org);
    isDynamic = org.isDynamic;
    outerRadius = org.outerRadius;
    filled = org.filled;
}

// SLOT FUNCTION(s)
//------------------------------------------------------------------------------
// setSlotIsDyanmic()
//------------------------------------------------------------------------------
bool DialArcSegment::setSlotIsDynamic(const base::Boolean* const newD)
{
    bool ok = false;
    if (newD != nullptr) ok = setIsDynamic(newD->asBool());
    return ok;
}
//------------------------------------------------------------------------------
// setSlotOuterRadius() - sets the OUTER dial radius
//------------------------------------------------------------------------------
bool DialArcSegment::setSlotOuterRadius(const base::INumber* const x)
{
    if (x != nullptr) {
        return setOuterRadius(x->asDouble());
    }
    return false;
}

//------------------------------------------------------------------------------
// setSlotFilled() - is our segment filled or not?
//------------------------------------------------------------------------------
bool DialArcSegment::setSlotFilled(const base::Boolean* const x)
{
    if (x != nullptr) {
        return setFilled(x->asBool());
    }
    return false;
}

// SET FUNCTIONS
//------------------------------------------------------------------------------
// setIsDynamic()
//------------------------------------------------------------------------------
bool DialArcSegment::setIsDynamic(const bool newID)
{
    isDynamic = newID;
    return true;
}

//------------------------------------------------------------------------------
// setOuterRadius() -
//------------------------------------------------------------------------------
bool DialArcSegment::setOuterRadius(const double x)
{
    if (x >= getRadius()) outerRadius = x;
    return true;
}

//------------------------------------------------------------------------------
// setFilled() -
//------------------------------------------------------------------------------
bool DialArcSegment::setFilled(const bool x)
{
    filled = x;
    return true;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void DialArcSegment::drawFunc()
{
    // get our data from our base class
    double startAngle = getStartAngle();
    double radius = getRadius();
    double sweepAngle = getSweepAngle();

    GLint curSlices = getSlices();
    // our slice amount should go up as we get more of a sweep, if not, it will
    // look funny.  Pretty much one slice per degree sweep
    double y = std::fabs(static_cast<double>(sweepAngle));
    curSlices = curSlices + static_cast<GLint>(y * 0.05f);

    // draw our arc
    glPushMatrix();
        GLUquadric* qobj = gluNewQuadric();
        if (filled) gluQuadricDrawStyle(qobj, GL_FILL);
        else gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
        gluPartialDisk(qobj, radius, outerRadius, curSlices, 1, startAngle, sweepAngle);
        gluDeleteQuadric(qobj);
    glPopMatrix();
}

//------------------------------------------------------------------------------
// onUpdateRadius() - event function to update our radius value
//------------------------------------------------------------------------------
bool DialArcSegment::onUpdateRadius(const base::INumber* const x)
{
    bool ok = false;
    if (x != nullptr) {
        // first determine our ratio from inner to outer radius
        double diff = outerRadius - getRadius();
        ok = setRadius(x->asDouble());
        outerRadius = getRadius() + diff;
    }
    return ok;
}

//------------------------------------------------------------------------------
// updateData()
//------------------------------------------------------------------------------
void DialArcSegment::updateData(const double dt)
{
    BaseClass::updateData(dt);

    if (isDynamic) setSweepAngle(getInstValue());
}

}
}
