
#include "mixr/base/colors/Hls.hpp"

#include "mixr/base/numeric/Number.hpp"
#include <cmath>

#include "mixr/base/util/platform_api.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Hls, "hls")
EMPTY_DELETEDATA(Hls)

BEGIN_SLOTTABLE(Hls)
    "hue",         // 1: ... Hue component, range(0.0 to 360.0)
    "lightness",   // 2: ... Lightness component, range(0.0 to 1.0)
    "saturation"   // 3: ... Saturation component, range(0.0 to 1.0)
END_SLOTTABLE(Hls)

BEGIN_SLOT_MAP(Hls)
    ON_SLOT(1, setSlotHue,        Number)
    ON_SLOT(2, setSlotLightness,  Number)
    ON_SLOT(3, setSlotSaturation, Number)
END_SLOT_MAP()

Hls::Hls(const double h, const double l, const double s)
{
   STANDARD_CONSTRUCTOR()
   hls[HUE]        = h;     // set the values
   hls[LIGHTNESS]  = l;
   hls[SATURATION] = s;
   hls2rgb(color,hls);      // set the rgb values
}

Hls::Hls()
{
   STANDARD_CONSTRUCTOR()
   hls[HUE]        = 0.0f;  // default to black
   hls[LIGHTNESS]  = 0.0f;
   hls[SATURATION] = 0.0f;
   hls2rgb(color,hls);   // set the rgb values
}

void Hls::copyData(const Hls& org, const bool)
{
   BaseClass::copyData(org);
   hls = org.hls;
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------
double Hls::hue() const
{
    return hls[HUE];
}

double Hls::saturation() const
{
    return hls[SATURATION];
}

double Hls::lightness() const
{
    return hls[LIGHTNESS];
}

void Hls::getHLS(Vec3d& hhh) const
{
    hhh.set(hls[HUE], hls[SATURATION], hls[LIGHTNESS]);
}

//------------------------------------------------------------------------------
// setSlotHue() -- set the hue value
//------------------------------------------------------------------------------
bool Hls::setSlotHue(const Number* const x)
{
    const double value{x->asDouble()};
    const bool ok{value >= 0 && value <= 360};
    if (ok) {
        hls[HUE] = value;
        hls2rgb(color,hls);
    } else {
        std::cerr << "Hls::setHue: invalid entry(" << value << "), valid range: 0 to 360" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotSaturation() -- set the saturation value
//------------------------------------------------------------------------------
bool Hls::setSlotSaturation(const Number* const x)
{
    const double value{x->asDouble()};
    const bool ok{value >= 0 && value <= 1};
    if (ok) {
        hls[SATURATION] = value;
        hls2rgb(color,hls);
    } else {
       std::cerr << "Hls::setSaturation: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotLightness() -- set the lightness value
//------------------------------------------------------------------------------
bool Hls::setSlotLightness(const Number* const x)
{
    const double value{x->asDouble()};
    const bool ok{value >= 0 && value <= 1};
    if (ok) {
        hls[LIGHTNESS] = value;
        hls2rgb(color,hls);
    } else {
        std::cerr << "Hls::setLightness: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}


//------------------------------------------------------------------------------
// value() -- utility function used in hls2rgb
//------------------------------------------------------------------------------
double Hls::value(double n1, double n2, double hue)
{
    if (hue > 360.0f) hue -= 360.0f;
    if (hue < 0.0f) hue += 360.0f;

    if (hue < 60.0)
        return(n1+(n2-n1)*(hue/60.0f));
    else if (hue < 180.0f)
        return(n2);
    else if (hue < 240.0f)
        return(n1+(n2-n1)*((240.0f-hue)/60.0f));
    else
        return(n1);
}

//------------------------------------------------------------------------------
// hls2rgb() -- converts a Hue, Saturation, Lightness (HLS) color to a
//              Red, Green, Blue (RGB) value.
//------------------------------------------------------------------------------
void Hls::hls2rgb(Vec4d& rgb, const Vec3d& hls)
{
    double m1{}, m2{};

    if (hls[LIGHTNESS] <= 0.5)
        m2 = hls[LIGHTNESS] * (1.0f + hls[SATURATION]);
    else
        m2 = hls[SATURATION] + hls[LIGHTNESS] * (1.0f - hls[SATURATION]);

    m1 = 2.0f * hls[LIGHTNESS] - m2;

    if (hls[SATURATION] == 0.0) {
        rgb[RED] = hls[LIGHTNESS];
        rgb[GREEN] = hls[LIGHTNESS];
        rgb[BLUE] = hls[LIGHTNESS];
    } else {
        rgb[RED]   = value( m1, m2, hls[HUE] + 120.0f );
        rgb[GREEN] = value( m1, m2, hls[HUE] );
        rgb[BLUE]  = value( m1, m2, hls[HUE] - 120.0f );
    }

    rgb[ALPHA] = defaultAlpha;
}

//------------------------------------------------------------------------------
// rgb2hls() -- converts a Red, Green, Blue (RGB) color to a
//              Hue, Saturation, Lightness (HLS) value.
//------------------------------------------------------------------------------
void Hls::rgb2hls(Vec3d& hls, const Vec4d& rgb)
{
    const double maxcol{std::fmax( rgb[RED], std::fmax( rgb[GREEN], rgb[BLUE] ) )};
    const double mincol{std::fmin( rgb[RED], std::fmin( rgb[GREEN], rgb[BLUE] ) )};
    hls[LIGHTNESS] = (mincol + maxcol) / 2.0f;

    if (maxcol == mincol) {
        hls[SATURATION] = 0.0f;
        hls[HUE] = 0.0f;
    } else {
        const double cdelta{maxcol - mincol};
        const double rc{( maxcol - rgb[RED] ) / cdelta};
        const double gc{( maxcol - rgb[GREEN] ) / cdelta};
        const double bc{( maxcol - rgb[BLUE] ) / cdelta};

        if (hls[LIGHTNESS] <= 0.5)
            hls[SATURATION] = cdelta / (maxcol + mincol);
        else
            hls[SATURATION] = cdelta / (2.0f - cdelta);

        if ( rgb[RED] == maxcol)
            hls[HUE] = bc - gc;
        else if ( rgb[GREEN] == maxcol )
            hls[HUE] = 2.0f + rc - bc;
        else
            hls[HUE] = 4.0f + gc - rc;
        hls[HUE] *= 60.0f;
        if (hls[HUE] < 0.0) hls[HUE] += 360.0f;
   }
}

}
}
