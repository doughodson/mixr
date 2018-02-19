
#ifndef __mixr_base_colors_Yiq_H__
#define __mixr_base_colors_Yiq_H__

#include "mixr/base/colors/Color.hpp"

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Yiq
// Description: Defines a YIQ color as used in NTSC
//
// Y defines the luminance, for a black-and-white image.
// I and Q determine the color.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: yiq
// Slots:
//       Y  <Number>  Y component (0.0 to 1.0) (default: 0.0)
//       I  <Number>  I component (-0.6 to 0.6) (default: 0.0)
//       Q  <Number>  Q component (-0.52.0 to 0.52) (default: 0.0)
//------------------------------------------------------------------------------
// Events: None
//------------------------------------------------------------------------------
// Notes: The operators Vec3d*() and Vec4d*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        YIQ color vector.
//------------------------------------------------------------------------------
class Yiq final: public Color
{
    DECLARE_SUBCLASS(Yiq, Color)

public:
    // components of YIQ color - used as an index into vector
    enum { Y, I, Q };

public:
    Yiq();
    Yiq(const double y, const double i, const double q);

    // return individual YIQ component.
    double y() const;
    double i() const;
    double q() const;
    void getYIQ(Vec3d& yiq) const;

    // static functions to convert YIQ colors to RGB and visa versa
    static void yiq2rgb(Vec4d& rgb, const Vec3d& yiq);
    static void rgb2yiq(Vec3d& yiq, const Vec4d& rgb);

protected:
    Vec3d yiq;

private:
   // slot table helper methods
   bool setSlotY(const Number* const);
   bool setSlotI(const Number* const);
   bool setSlotQ(const Number* const);
};

}
}

#endif
