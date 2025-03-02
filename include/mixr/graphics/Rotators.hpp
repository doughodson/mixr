
#ifndef __mixr_graphics_Rotators_HPP__
#define __mixr_graphics_Rotators_HPP__

#include "Graphic.hpp"

#include "mixr/base/units/util/angle_utils.hpp"

namespace mixr {
namespace base { class IAngle; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: Rotators
//
// Description: A simple graphic that rotates, and can accept either degrees
// or radians, about 3 axes.
//
// Factory name: Rotators
//  Events messages:
//      UPDATE_VALUE - rotates around the X axis - radians (Angle or Number)
//      UPDATE_VALUE2 - rotates around the X axis - degrees (Number only)
//      UPDATE_VALUE3 - rotates around the Y axis - radians (Angle or Number)
//      UPDATE_VALUE4 - rotates around the Y axis - degrees (Number only)
//      UPDATE_VALUE5 - rotates around the Z axis - radians (Angle or Number)
//      UPDATE_VALUE6 - rotates around the Z axis - degrees (Number only)
//
// Note: Since we convert statically to radians any time we pass an Angle as
// an event argument, there is no need to have an event function that converts
// an Angle to degrees.  You can pass a 'Degrees' object as an event argument using
// UPDATE_VALUE, and it will automatically convert to radians.  That is why there
// is no onXRotateDeg with an Angle argument.
//------------------------------------------------------------------------------
class Rotators : public Graphic
{
    DECLARE_SUBCLASS(Rotators, Graphic)

public:
    Rotators();

    // Get functions
    double getXRotation()    const  { return xRot; }
    double getXRotationDeg() const  { return xRot * static_cast<double>(base::angle::R2DCC); }
    double getYRotation()    const  { return yRot; }
    double getYRotationDeg() const  { return yRot * static_cast<double>(base::angle::R2DCC); }
    double getZRotation()    const  { return zRot; }
    double getZRotationDeg() const  { return zRot * static_cast<double>(base::angle::R2DCC); }
    // get all angles
    void getRotationAngles(double& x, double& y, double& z);

    // Set functions
    bool setXRotation(const double angle);
    bool setXRotationDeg(const double angle);
    bool setYRotation(const double angle);
    bool setYRotationDeg(const double angle);
    bool setZRotation(const double angle);
    bool setZRotationDeg(const double angle);

    bool setRotations(const double x, const double y = 0, const double z = 0);
    bool setRotationsDeg(const double x, const double y = 0, const double z = 0);

    void draw() override;
    bool event(const int event, base::IObject* const obj = nullptr) override;

private:
    // Event callbacks
    bool onXRotate(const base::IAngle* const);
    bool onXRotate(const base::INumber* const);
    bool onXRotateDeg(const base::INumber* const);
    bool onYRotate(const base::IAngle* const);
    bool onYRotate(const base::INumber* const);
    bool onYRotateDeg(const base::INumber* const);
    bool onZRotate(const base::IAngle* const);
    bool onZRotate(const base::INumber* const);
    bool onZRotateDeg(const base::INumber* const);

    double xRot{-1.0};   // x axis rotation
    double yRot{-1.0};   // y axis rotation
    double zRot{-1.0};   // z axis rotation
};

}
}

#endif
