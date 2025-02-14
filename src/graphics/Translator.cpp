
#include "mixr/graphics/Translator.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include <iostream>

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Translator, "Translator")
EMPTY_SLOTTABLE(Translator)
EMPTY_DELETEDATA(Translator)

BEGIN_EVENT_HANDLER(Translator)
    ON_EVENT_OBJ(UPDATE_VALUE,  onUpdateX, base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE2, onUpdateY, base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE3, onUpdateZ, base::INumber)
END_EVENT_HANDLER()

Translator::Translator()
{
    STANDARD_CONSTRUCTOR()
}

void Translator::copyData(const Translator& org, const bool)
{
    BaseClass::copyData(org);

    myXPos = org.myXPos;
    myYPos = org.myYPos;
    myZPos = org.myZPos;
}

//------------------------------------------------------------------------------
// draw() -- draws the object(s)
//------------------------------------------------------------------------------
void Translator::draw()
{
    // we are just translating here
    glPushMatrix();
        glTranslated(static_cast<double>(myXPos), static_cast<double>(myYPos), static_cast<double>(myZPos));
        Graphic::draw();
    glPopMatrix();
}

// EVENT functions
//------------------------------------------------------------------------------
// onUpdateX() - update our x position (inches)
//------------------------------------------------------------------------------
bool Translator::onUpdateX(const base::INumber* const newX)
{
    bool ok {};
    if (newX != nullptr) ok = setXPos(newX->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// onUpdateY() - update our y position (inches)
//------------------------------------------------------------------------------
bool Translator::onUpdateY(const base::INumber* const newY)
{
    bool ok {};
    if (newY != nullptr) ok = setYPos(newY->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
// onUpdateY() - update our z position (inches)
//------------------------------------------------------------------------------
bool Translator::onUpdateZ(const base::INumber* const newZ)
{
    bool ok {};
    if (newZ != nullptr) ok = setZPos(newZ->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// functions for setting member variables
//------------------------------------------------------------------------------
bool Translator::setXPos(const double xPos)
{
    myXPos = xPos;
    return true;
}

bool Translator::setYPos(const double yPos)
{
    myYPos = yPos;
    return true;
}

bool Translator::setZPos(const double zPos)
{
    myZPos = zPos;
    return true;
}

}
}
