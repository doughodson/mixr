
#ifndef __mixr_base_colors_Hsva_HPP__
#define __mixr_base_colors_Hsva_HPP__

#include "mixr/base/colors/Hsv.hpp"

namespace mixr {
namespace base {
class INumber;

//------------------------------------------------------------------------------
// Class: Hsva
// Description: Defines a color by Hue, Saturation, Value and Alpha
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: hsva
// Slots:
//    alpha   <INumber>  ! Set the alpha component (0.0 to 1.0) (default: 1.0)
//------------------------------------------------------------------------------
// Notes: The operators Vec3d*() and Vec4d*(), (inherited from Color)
//        return a const pointer to the RGBA color vector and not the
//        HSVA color vector.
//------------------------------------------------------------------------------
class Hsva final: public Hsv
{
    DECLARE_SUBCLASS(Hsva, Hsv)

public:
    Hsva(const double h, const double s, const double v, const double a);
    Hsva();

    // returns the HSVA components in a vector
    void getHSVA(Vec4d& hsva) const   { Hsv::getHSVA(hsva);        }
    // sets the HSVA components in a vector
    bool setHSVA(const Vec4d& vec)    { return Hsv::setHSVA(vec);  }

    // Color map interpolation - given a value, min & max limit, and HSV colors
    // for the min & max limits.  This routine will interpolate the HSV colors
    // between the min & max limits to find the HSV color for the given value.
    bool colorInterpolate(
      const double value,      // Value
      const double minValue,   // Minimum Value
      const double maxValue,   // Maximum Value
      const Hsva& minColor,    // Minimum HSV color
      const Hsva& maxColor     // Minimum HSV color
    );

private:
   // slot table helper methods
   bool setSlotAlpha(const INumber* const);
};

}
}

#endif
