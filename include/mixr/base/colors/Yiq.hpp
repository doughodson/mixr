
#ifndef __mixr_base_colors_Yiq_HPP__
#define __mixr_base_colors_Yiq_HPP__

#include "mixr/base/colors/IColor.hpp"

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Yiq
//
// Description: YIQ is a color space used bt the NTSC color TV system
//
// Y: defines the luma information (i.e., luminance) and is the only
//    component used in a black-and-white television receiver
//
// I (inphase) and Q (quadrature) represent the chrominance information
// which can be thought of as an X/Y axis that determines the color
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Yiq
//
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
class Yiq final: public IColor
{
    DECLARE_SUBCLASS(Yiq, IColor)

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
