
#include "mixr/base/colors/Color.hpp"

#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Color, "AbstractColor")
EMPTY_SLOTTABLE(Color)
EMPTY_DELETEDATA(Color)

double Color::defaultAlpha{1.0};

Color::Color()
{
    STANDARD_CONSTRUCTOR()
    // default color is black
    color[RED]   = 0.0;
    color[GREEN] = 0.0;
    color[BLUE]  = 0.0;
    color[ALPHA] = defaultAlpha;
}

Color::Color(const double red, const double green, const double blue, const double alpha)
{
    STANDARD_CONSTRUCTOR()
    color[RED]   = red;
    color[GREEN] = green;
    color[BLUE]  = blue;
    color[ALPHA] = alpha;
}

void Color::copyData(const Color& org, const bool)
{
   BaseClass::copyData(org);
   color = org.color; 
}

Color::operator const Vec3d*() const
{
    // Note: Color is a Vec4, which is just a four element array [ r g b a ], and
    // we're reinterpreting it as a Vec3, which is a three element array [ r g b ].
    return reinterpret_cast<const Vec3d*>( &color );
}

Color::operator const Vec4d*() const
{
    return &color;
}

// Return the color vector of color array index PF_RED
double Color::red() const
{
    return color[RED];
}

// Return the color vector of color array index PF_GREEN
double Color::green() const
{
    return color[GREEN];
}

// Return the color vector of color array index PF_BLUE
double Color::blue() const
{
    return color[BLUE];
}

// Return the color vector of color array index PF_ALPHA
double Color::alpha() const
{
    return color[ALPHA];
}

// Get the address of the color vector array(3)
const Vec3d* Color::getRGB() const
{
    // Note: Color is a Vec4, which is just a four element array [ r g b a ], and
    // we're reinterpreting it as a Vec3, which is a three element array [ r g b ].
    return reinterpret_cast<const Vec3d*>( &color );
}

// Get the address of the color vector array(4)
const Vec4d* Color::getRGBA() const
{
    return &color;
}

bool Color::setRed(const double value)
{
    bool ok{value >= 0 && value <= 1};
    if (ok) color[Color::RED] = value;
    return ok;
}

bool Color::setGreen(const double value)
{
    bool ok{value >= 0 && value <= 1};
    if (ok) color[Color::GREEN] = value;
    return ok;
}

bool Color::setBlue(const double value)
{
    bool ok{value >= 0 && value <= 1};
    if (ok) color[Color::BLUE] = value;
    return ok;
}

bool Color::setAlpha(const double value)
{
    bool ok{value >= 0 && value <= 1};
    if (ok) color[Color::ALPHA] = value;
    return ok;
}

bool operator==(const Color& c1, const Color& c2)
{
    return (c1.getRGBA() == c2.getRGBA());
}

bool operator!=(const Color& c1, const Color& c2)
{
    return (c1.getRGBA() != c2.getRGBA());
}

// returns the default alpha value
double Color::getDefaultAlpha()
{
   return defaultAlpha;
}

void Color::setDefaultAlpha(const double alpha)
{
    defaultAlpha = alpha;
}

}
}
