
#include "mixr/graphics/Rotators.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/qty/angles.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Rotators, "Rotators")
EMPTY_SLOTTABLE(Rotators)
EMPTY_DELETEDATA(Rotators)

BEGIN_EVENT_HANDLER(Rotators)
    ON_EVENT_OBJ(UPDATE_VALUE,  onXRotate,    base::IAngle)
    ON_EVENT_OBJ(UPDATE_VALUE,  onXRotate,    base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE2, onXRotateDeg, base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE3, onYRotate,    base::IAngle)
    ON_EVENT_OBJ(UPDATE_VALUE3, onYRotate,    base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE4, onYRotateDeg, base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE5, onZRotate,    base::IAngle)
    ON_EVENT_OBJ(UPDATE_VALUE5, onZRotate,    base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE6, onZRotateDeg, base::INumber)
END_EVENT_HANDLER()

Rotators::Rotators()
{
    STANDARD_CONSTRUCTOR()
}

void Rotators::copyData(const Rotators& org, const bool)
{
    BaseClass::copyData(org);

    xRot = org.xRot;
    yRot = org.yRot;
    zRot = org.zRot;
}

//------------------------------------------------------------------------------
// setXRotation() -- Sets the X rotation angle (rad)
//------------------------------------------------------------------------------
bool Rotators::setXRotation(const double angle)
{
    xRot = angle;
    return true;
}

//------------------------------------------------------------------------------
// setXRotationDeg() -- Sets the rotation angle (deg)
//------------------------------------------------------------------------------
bool Rotators::setXRotationDeg(const double angle)
{
    xRot = angle * static_cast<double>(base::angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setYRotation() -- Sets the Y rotation angle (rad)
//------------------------------------------------------------------------------
bool Rotators::setYRotation(const double angle)
{
    yRot = angle;
    return true;
}

//------------------------------------------------------------------------------
// setYRotationDeg() -- Sets the Y rotation angle (deg)
//------------------------------------------------------------------------------
bool Rotators::setYRotationDeg(const double angle)
{
    yRot = angle * static_cast<double>(base::angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setZRotationDeg() -- Sets the Z rotation angle (deg)
//------------------------------------------------------------------------------
bool Rotators::setZRotationDeg(const double angle)
{
    zRot = angle * static_cast<double>(base::angle::D2RCC);
    return true;
}

//------------------------------------------------------------------------------
// setZRotation() -- Sets the Z rotation angle (rad)
//------------------------------------------------------------------------------
bool Rotators::setZRotation(const double angle)
{
    zRot = angle;
    return true;
}

//------------------------------------------------------------------------------
// setRotations() -- set all of our rotations
//------------------------------------------------------------------------------
bool Rotators::setRotations(const double x, const double y, const double z)
{
    xRot = x;
    yRot = y;
    zRot = z;
    return true;
}

//------------------------------------------------------------------------------
// setRotationsDeg() -- set all of our rotations (deg)
//------------------------------------------------------------------------------
bool Rotators::setRotationsDeg(const double x, const double y, const double z)
{
    xRot = x * static_cast<double>(base::angle::D2RCC);
    yRot = y * static_cast<double>(base::angle::D2RCC);
    zRot = z * static_cast<double>(base::angle::D2RCC);
    return true;
}


//------------------------------------------------------------------------------
// onXRotate() -- updates the X rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onXRotate(const base::INumber* const rotation)
{
    if (rotation != nullptr) return setXRotation( rotation->asDouble() );
    return false;
}

//------------------------------------------------------------------------------
// onXRotate() -- updates the X rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onXRotate(const base::IAngle* const rotation)
{
    if (rotation != nullptr)
        return setXRotation(rotation->getValueInRadians());
    return false;
}

//------------------------------------------------------------------------------
// onXRotateDeg() -- updates the X rotation value (deg)
//------------------------------------------------------------------------------
bool Rotators::onXRotateDeg(const base::INumber* const rotation)
{
    if (rotation != nullptr) return setXRotationDeg( rotation->asDouble() );
    return false;
}

//------------------------------------------------------------------------------
// onYRotate() -- updates the Y rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onYRotate(const base::INumber* const rotation)
{
    if (rotation != nullptr) return setYRotation(rotation->asDouble());
    return false;
}

//------------------------------------------------------------------------------
// onYRotate() -- updates the Y rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onYRotate(const base::IAngle* const rotation)
{
    if (rotation != nullptr)
        return setYRotation(rotation->getValueInRadians());
    return false;
}

//------------------------------------------------------------------------------
// onYRotateDeg() -- updates the Y rotation value (deg)
//------------------------------------------------------------------------------
bool Rotators::onYRotateDeg(const base::INumber* const rotation)
{
    if (rotation != nullptr) return setYRotationDeg(rotation->asDouble());
    return false;
}

//------------------------------------------------------------------------------
// onZRotate() -- updates the Z rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onZRotate(const base::INumber* const rotation)
{
    if (rotation != nullptr) return setZRotation(rotation->asDouble());
    return false;
}

//------------------------------------------------------------------------------
// onZRotate() -- updates the Z rotation value (rad)
//------------------------------------------------------------------------------
bool Rotators::onZRotate(const base::IAngle* const rotation)
{
    if (rotation != nullptr)
        return setZRotation(rotation->getValueInRadians());
    return false;
}

//------------------------------------------------------------------------------
// onZRotateDeg() -- updates the Z rotation value (deg)
//------------------------------------------------------------------------------
bool Rotators::onZRotateDeg(const base::INumber* const rotation)
{
    if (rotation != nullptr) return setZRotationDeg( rotation->asDouble() );
    return false;
}

//------------------------------------------------------------------------------
// drawFunc() -- draws the object(s)
//------------------------------------------------------------------------------
void Rotators::draw()
{
    lcSaveMatrix();
    if (xRot != -1) lcRotate(1, 0, 0, xRot);
    if (yRot != -1) lcRotate(0, 1, 0, yRot);
    if (zRot != -1) lcRotate(0, 0, 1, zRot);
    Graphic::draw();
    lcRestoreMatrix();
}

}
}


