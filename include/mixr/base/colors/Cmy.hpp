
#ifndef __mixr_base_colors_Cmy_H__
#define __mixr_base_colors_Cmy_H__

#include "mixr/base/colors/Color.hpp"

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Cmy
// Description: Defines a color by Cyan, Magenta, and Yellow.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: cmy
// Slots:
//    cyan     <Number>  ! Cyan component (0.0 to 1.0) (default: 0)
//    magenta  <Number>  ! Magenta component (0.0 to 1.0) (default: 0)
//    yellow   <Number>  ! Yellow component (0.0 to 1.0) (default: 0)
//------------------------------------------------------------------------------
// Notes: The operators Vec3d*() and Vec4d*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        CMY color vector.
//------------------------------------------------------------------------------
class Cmy final: public Color
{
    DECLARE_SUBCLASS(Cmy, Color)

public:
    // components of CMY color - used to index color vector
    enum { CYAN, MAGENTA, YELLOW };

public:
    Cmy();
    Cmy(const double c, const double m, const double y);

    // returns individual CMY components
    double cyan() const;
    double magenta() const;
    double yellow() const;

    // returns the CMY components in a vector
    void getCMY(Vec3d& cmy) const;

    // static functions to convert CMY colors to RGB and visa versa
    static void cmy2rgb(Vec4d& rgb, const Vec3d& cmy);
    static void rgb2cmy(Vec3d& cmy, const Vec4d& rgb);

protected:
    Vec3d cmy;

private:
   // slot table helper methods
   bool setSlotCyan(const Number* const);
   bool setSlotMagenta(const Number* const);
   bool setSlotYellow(const Number* const);
};

}
}

#endif
