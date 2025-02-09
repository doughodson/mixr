
#include "mixr/base/colors/IColor.hpp"

#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(IColor, "IColor")
EMPTY_SLOTTABLE(IColor)
EMPTY_DELETEDATA(IColor)

double IColor::defaultAlpha{1.0};

IColor::IColor()
{
    STANDARD_CONSTRUCTOR()
    // default color is black
    color[RED]   = 0.0;
    color[GREEN] = 0.0;
    color[BLUE]  = 0.0;
    color[ALPHA] = defaultAlpha;
}

IColor::IColor(const double red, const double green, const double blue, const double alpha)
{
    STANDARD_CONSTRUCTOR()
    color[RED]   = red;
    color[GREEN] = green;
    color[BLUE]  = blue;
    color[ALPHA] = alpha;
}

void IColor::copyData(const IColor& org, const bool)
{
   BaseClass::copyData(org);
   color = org.color;
}

IColor::operator const Vec3d*() const
{
    // Note: Color is a Vec4, which is just a four element array [ r g b a ], and
    // we're reinterpreting it as a Vec3, which is a three element array [ r g b ].
    return reinterpret_cast<const Vec3d*>( &color );
}

IColor::operator const Vec4d*() const
{
    return &color;
}

// Return the color vector of color array index PF_RED
double IColor::red() const
{
    return color[RED];
}

// Return the color vector of color array index PF_GREEN
double IColor::green() const
{
    return color[GREEN];
}

// Return the color vector of color array index PF_BLUE
double IColor::blue() const
{
    return color[BLUE];
}

// Return the color vector of color array index PF_ALPHA
double IColor::alpha() const
{
    return color[ALPHA];
}

// Get the address of the color vector array(3)
const Vec3d* IColor::getRGB() const
{
    // Note: Color is a Vec4, which is just a four element array [ r g b a ], and
    // we're reinterpreting it as a Vec3, which is a three element array [ r g b ].
    return reinterpret_cast<const Vec3d*>( &color );
}

// Get the address of the color vector array(4)
const Vec4d* IColor::getRGBA() const
{
    return &color;
}

bool IColor::setRed(const double value)
{
    bool ok{value >= 0 && value <= 1};
    if (ok) color[IColor::RED] = value;
    return ok;
}

bool IColor::setGreen(const double value)
{
    bool ok{value >= 0 && value <= 1};
    if (ok) color[IColor::GREEN] = value;
    return ok;
}

bool IColor::setBlue(const double value)
{
    bool ok{value >= 0 && value <= 1};
    if (ok) color[IColor::BLUE] = value;
    return ok;
}

bool IColor::setAlpha(const double value)
{
    bool ok{value >= 0 && value <= 1};
    if (ok) color[IColor::ALPHA] = value;
    return ok;
}

bool operator==(const IColor& c1, const IColor& c2)
{
    return (c1.getRGBA() == c2.getRGBA());
}

bool operator!=(const IColor& c1, const IColor& c2)
{
    return (c1.getRGBA() != c2.getRGBA());
}

// returns the default alpha value
double IColor::getDefaultAlpha()
{
   return defaultAlpha;
}

void IColor::setDefaultAlpha(const double alpha)
{
    defaultAlpha = alpha;
}

}
}
