
#include "mixr/base/colors/Hsv.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/angles.hpp"
#include "mixr/base/util/math_utils.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"

#include <cmath>

#include "mixr/base/util/platform_api.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Hsv, "hsv")
EMPTY_DELETEDATA(Hsv)

BEGIN_SLOTTABLE(Hsv)
    "hue",          // 1: hue component,        range(0.0 to 360.0)
    "saturation",   // 2: saturation component, range(0.0 to 1.0)
    "value"         // 3: value component,      range(0.0 to 1.0)
END_SLOTTABLE(Hsv)

BEGIN_SLOT_MAP(Hsv)
    ON_SLOT(1, setSlotHue,        Number)
    ON_SLOT(2, setSlotSaturation, Number)
    ON_SLOT(3, setSlotValue,      Number)
END_SLOT_MAP()

Hsv::Hsv(const double h, const double s, const double v)
{
   STANDARD_CONSTRUCTOR()
   hsv[HUE]        = h;     // set the values
   hsv[SATURATION] = s;
   hsv[VALUE]      = v;
   hsv[ALPHA]      = getDefaultAlpha();
   hsv2rgb(color,hsv);      // set the rgb values
}

Hsv::Hsv()
{
   STANDARD_CONSTRUCTOR()
   hsv[HUE]        = 0.0f;  // default to black
   hsv[SATURATION] = 0.0f;
   hsv[VALUE]      = 0.0f;
   hsv[ALPHA]      = getDefaultAlpha();
   hsv2rgb(color,hsv);      // set the rgb values
}

void Hsv::copyData(const Hsv& org, const bool)
{
   BaseClass::copyData(org);
   hsv = org.hsv;
}

//------------------------------------------------------------------------------
// data access functions
//------------------------------------------------------------------------------
double Hsv::hue() const
{
    return hsv[HUE];
}

double Hsv::saturation() const
{
    return hsv[SATURATION];
}

double Hsv::value() const
{
    return hsv[VALUE];
}

void Hsv::getHSV(Vec3d& hhh) const
{
    hhh.set(hsv[HUE],hsv[SATURATION],hsv[VALUE]);
}

void Hsv::getHSVA(Vec4d& hhh) const
{
    hhh.set(hsv[HUE],hsv[SATURATION],hsv[VALUE],hsv[ALPHA]);
}

//------------------------------------------------------------------------------
// setSlotHue() -- set the HSV hue
//------------------------------------------------------------------------------
bool Hsv::setSlotHue(const Number* const x)
{
    const double value{x->asDouble()};
    const bool ok{value >= 0.0 && value <= 360.0};
    if (ok) {
        hsv[HUE] = value;
        hsv2rgb(color,hsv);
    } else {
        std::cerr << "Hsv::setHue: invalid entry(" << value << "), valid range: 0 to 360" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotSaturation() -- set the HSV saturation
//------------------------------------------------------------------------------
bool Hsv::setSlotSaturation(const Number* const x)
{
    const double value{x->asDouble()};
    const bool ok{value >= 0.0 && value <= 1.0};
    if (ok) {
        hsv[SATURATION] = value;
        hsv2rgb(color,hsv);
    } else {
        std::cerr << "Hsv::setSaturation: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotValue() -- set the HSV value
//------------------------------------------------------------------------------
bool Hsv::setSlotValue(const Number* const x)
{
    const double value{x->asDouble()};
    const bool ok{value >= 0.0 && value <= 1.0};
    if (ok) {
        hsv[VALUE] = value;
        hsv2rgb(color,hsv);
    } else {
        std::cerr << "Hsv::setValue: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setHSV() -- Sets the hsv vector
//------------------------------------------------------------------------------
bool Hsv::setHSV(const Vec3d& vec)
{
   hsv[0] = vec[0];
   hsv[1] = vec[1];
   hsv[2] = vec[2];
   hsv2rgb(color,hsv);
   return true;
}

bool Hsv::setHSVA(const Vec4d& vec)
{
   hsv = vec;
   hsv2rgb(color,hsv);
   return true;
}

//------------------------------------------------------------------------------
// hsv2rgb() -- converts a Hue, Saturation, Value (HSV) color value to an
//              Red, Green, Blue (RGB) value
//
// This code is based on '/usr/people/4Dgifts/iristools/libgutil/colormod.c'
//------------------------------------------------------------------------------
void Hsv::hsv2rgb(Vec3d& rgb, const Vec3d& hsv)
{
    // local HSV values
    double h{angle::aepcdDeg(hsv[HUE])};
    if (h < 0.0f) h += 360.0f;
    double s{hsv[SATURATION]};
    double v{hsv[VALUE]};

    if (s != 0.0) {

        // The max hue value is the same as the min hue value.
        if (h == 360.0f) h = 0.0f;
        h /= 60.0f;

        // computer some parameters
        const int i{static_cast<int>(h)};
        const double f{h - static_cast<double>(i)};
        const double p{v * (1.0f - s)};
        const double q{v * (1.0f - (s * f))};
        const double t{v * (1.0f - (s * (1.0f - f)))};

        switch (i) {
            case 0 : {
                // when hue is >= red and < yellow
                rgb[RED] = v;
                rgb[GREEN] = t;
                rgb[BLUE] = p;
            }
            break;

            case 1 : {
                // when hue is >= yellow and < green
                rgb[RED] = q;
                rgb[GREEN] = v;
                rgb[BLUE] = p;
            }
            break;

            case 2 : {
                // when hue is >= green and < cyan
                rgb[RED] = p;
                rgb[GREEN] = v;
                rgb[BLUE] = t;
            }
            break;

            case 3 : {
                // when hue is >= cyan and < blue
                rgb[RED] = p;
                rgb[GREEN] = q;
                rgb[BLUE] = v;
            }
            break;

            case 4 : {
                // when hue is >= blue and < magenta
                rgb[RED] = t;
                rgb[GREEN] = p;
                rgb[BLUE] = v;
            }
            break;

            case 5 : {
                // when hue is >= magenta and < red
                rgb[RED] = v;
                rgb[GREEN] = p;
                rgb[BLUE] = q;
            }
            break;
        }
    } else {
        // when saturation is zero, the color is gray of intensity 'v'
        rgb[RED] = v;
        rgb[GREEN] = v;
        rgb[BLUE] = v;
    }
}

void Hsv::hsv2rgb(Vec4d& rgb, const Vec4d& hsv)
{
   Vec3d hsv3(hsv[0], hsv[1], hsv[2]);
   Vec3d rgb3;
   hsv2rgb(rgb3, hsv3);

   // copy to output
   rgb[RED]   = rgb3[RED];
   rgb[GREEN] = rgb3[GREEN];
   rgb[BLUE]  = rgb3[BLUE];

   // just pass alpha
   rgb[ALPHA] = hsv[ALPHA];
}

//------------------------------------------------------------------------------
// rgb2hsv() -- converts a Red, Gree, Blue (RGB) color value to an
//              Hue, Saturation, Value (HSV) value.
//
// This code is based on '/usr/people/4Dgifts/iristools/libgutil/colormod.c'
//------------------------------------------------------------------------------
void Hsv::rgb2hsv(Vec3d& hsv, const Vec3d& rgb)
{
   const double cmax{std::fmax( rgb[RED], std::fmax(rgb[GREEN],rgb[BLUE]) )};
   const double cmin{std::fmin( rgb[RED], std::fmin(rgb[GREEN],rgb[BLUE]) )};
   const double cdelta{cmax - cmin};
   double h{};
   double s{};

   if ( cmax != 0.0 )
      s = cdelta / cmax;

   if ( s != 0.0 ) {
      const double rc{(cmax - rgb[RED]) / cdelta};
      const double gc{(cmax - rgb[GREEN]) / cdelta};
      const double bc{(cmax - rgb[BLUE]) / cdelta};

      if ( rgb[RED] == cmax )
         h = bc - gc;
      else if ( rgb[GREEN] == cmax )
         h = 2.0f + rc - bc;
      else if ( rgb[BLUE] == cmax )
         h = 4.0f + gc - rc;

      h *= 60.0f;
      if ( h < 0.0 )
         h += 360.0f;
   }

   hsv[HUE] = h;
   hsv[VALUE] = cmax;
   hsv[SATURATION] = s;
}

void Hsv::rgb2hsv(Vec4d& hsv, const Vec4d& rgb)
{
   // let the Vec3 version do the work
   Vec3d hsv3;
   Vec3d rgb3(rgb[0], rgb[1], rgb[2]);
   rgb2hsv(hsv3,rgb3);

   // copy to output (just pass alpha)
   hsv[HUE]        = hsv3[HUE];
   hsv[VALUE]      = hsv3[VALUE];
   hsv[SATURATION] = hsv3[SATURATION];

   // just pass alpha
   hsv[ALPHA] = rgb[ALPHA];
}

}
}
