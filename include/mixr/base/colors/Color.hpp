
#ifndef __mixr_base_colors_Color_H__
#define __mixr_base_colors_Color_H__

#include "mixr/base/Object.hpp"
#include "mixr/base/osg/Vec4d"

namespace mixr {
namespace base {
class Vec3d;
class Number;

//------------------------------------------------------------------------------
// Class: Color
// Description: General Purpose Color class; used with RGB and HSV classes
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Color
// Slots: (none)
//------------------------------------------------------------------------------
class Color : public Object
{
    DECLARE_SUBCLASS(Color, Object)

public:
    // components of RGB color - used as an index
    enum { RED, GREEN, BLUE, ALPHA };

public:
    Color();

    // convert a color to an Vec3d* RGB vector
    explicit operator const Vec3d*() const;
    // convert a color to an Vec4d* RGB vector
    explicit operator const Vec4d*() const;

    // data access routines.  Return the individual color components
    // as doubles with a range of 0.0 to 1.0.
    double red() const;
    double green() const;
    double blue() const;
    double alpha() const;

    virtual bool setRed(const double);
    virtual bool setGreen(const double);
    virtual bool setBlue(const double);
    virtual bool setAlpha(const double);

    // convert a color to a Vec3d* or Vec4d* RGB vector
    const Vec3d* getRGB() const;
    const Vec4d* getRGBA() const;

    // returns default alpha value
    static double getDefaultAlpha();
    // sets the default alpha value
    static void setDefaultAlpha(const double);

protected:
    Vec4d color;                  // RGBA color vector
    static double defaultAlpha;
};

}
}

#endif
