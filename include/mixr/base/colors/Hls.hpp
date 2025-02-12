
#ifndef __mixr_base_colors_Hls_HPP__
#define __mixr_base_colors_Hls_HPP__

#include "mixr/base/colors/IColor.hpp"

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Hls
//
// Description: Defines a color by Hue, Lightness and Saturation
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: hls
//
// Slots:
//     hue         <Number>  ! hue component (0.0 to 360.0, default: 0.0)
//     lightness   <Number>  ! lightness component (0.0 to 1.0, default: 0.0)
//     saturation  <Number>  ! saturation component (0.0 to 1.0, default: 0.0)
//------------------------------------------------------------------------------
// Notes: The operators Vec3d*() and Vec4d*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        HLS color vector.
//------------------------------------------------------------------------------
class Hls final: public IColor
{
    DECLARE_SUBCLASS(Hls, IColor)

public:
    // components of HLS color - used to index the HLS color vectors
    enum { HUE, LIGHTNESS, SATURATION };

public:
    Hls(const double h, const double l, const double s);
    Hls();

    // returns individual HLS components
    double hue() const;
    double lightness() const;
    double saturation() const;
    // returns the HLS components in a vector
    void getHLS(Vec3d& hls) const;

    // static functions to convert HLS colors to RGB and visa versa
    static void hls2rgb(Vec4d& rgb, const Vec3d& hls);
    static void rgb2hls(Vec3d& hls, const Vec4d& rgb);

protected:
    Vec3d hls;

private:
   static double value(double n1, double n2, double hue);

private:
   // slot table helper methods
   bool setSlotHue(const Number* const);
   bool setSlotLightness(const Number* const);
   bool setSlotSaturation(const Number* const);
};

}
}

#endif
