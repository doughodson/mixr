
#ifndef __mixr_base_colors_Hsv_HPP__
#define __mixr_base_colors_Hsv_HPP__

#include "mixr/base/colors/IColor.hpp"
#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Vec4d"

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Hsv
//
// Description: Defines a color by the hue, saturation and value model which more
//              closely aligns with the way human vision perceives color-making
//              attributes
//
//  Hues: degrees around a color wheel
//      red          0.0
//      yellow      60.0
//      green      120.0
//      cyan       180.0
//      blue       240.0
//      magenta    300.0
//      red        360.0
//
//  Saturation:
//      pure hue       1.0
//      no hue (white) 0.0
//
//  Value:
//      max intensity        1.0
//      no intensity (black) 0.0
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Hsv
//
// Slots:
//     hue        <Number>  ! Set the hue component (0.0 to 360.0, default: 0.0)
//     saturation <Number>  ! Set the saturation component (0.0 to 1.0, default: 0.0)
//     value      <Number>  ! Set the value component (0.0 to 1.0, default: 0.0)
//------------------------------------------------------------------------------
// Notes: The operators Vec3d*() and Vec4d*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        HSVA color vector.
//------------------------------------------------------------------------------
class Hsv : public IColor
{
    DECLARE_SUBCLASS(Hsv, IColor)

public:
    // components of HSV color - used as an index into a vector
    enum { HUE, SATURATION, VALUE, /* ALPHA */ };

public:
    Hsv(const double h, const double s, const double v);
    Hsv();

    // returns individual HSV components
    double hue() const;                          // Hue value (0.0 to 360.0) (see Hsv.hpp)
    double saturation() const;                   // Saturation: 0.0 (white) to 1.0 (pure color)
    double value() const;                        // Value: 0.0 (black) to 1.0 (full)

    void getHSV(Vec3d& vec) const;               // Returns the hsv vector
    bool setHSV(const Vec3d& vec);               // Sets the hsv vector

    // static convenience functions to convert HSVA colors to RGBA and visa versa
    static void hsv2rgb(Vec4d& rgb, const Vec4d& hsv); // Converts an HSVA color vector to a RGBA color vector
    static void hsv2rgb(Vec3d& rgb, const Vec3d& hsv); // Converts an HSV color vector to a RGB color vector

    static void rgb2hsv(Vec4d& hsv, const Vec4d& rgb); // Converts a RGBA color vector to a HSVA color vector
    static void rgb2hsv(Vec3d& hsv, const Vec3d& rgb); // Converts a RGB color vector to a HSV color vector

protected:
    void getHSVA(Vec4d& hsva) const;
    bool setHSVA(const Vec4d& vec);

    Vec4d hsv;

private:
    bool setSlotHue(const Number* const);
    bool setSlotSaturation(const Number* const);
    bool setSlotValue(const Number* const);
};

}
}

#endif
