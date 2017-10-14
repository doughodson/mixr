
#include "mixr/base/colors/Hsva.hpp"

#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/units/Angles.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Hsva, "hsva")
EMPTY_COPYDATA(Hsva)
EMPTY_DELETEDATA(Hsva)

BEGIN_SLOTTABLE(Hsva)
    "alpha",    // alpha component,   range(0.0f .. 1.0f)
END_SLOTTABLE(Hsva)

BEGIN_SLOT_MAP(Hsva)
    ON_SLOT(1, setSlotAlpha, Number)
END_SLOT_MAP()

Hsva::Hsva(const double h, const double s,
           const double v, const double a) : Hsv(h,s,v)
{
    STANDARD_CONSTRUCTOR()
    hsv[ALPHA] = a;
}

Hsva::Hsva()
{
    STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// Color map interpolation - given a value, min & max limit,
// and HSV colors for the min & max limits.  This routine will
// interpolate the HSV colors between the min & max limits to
// find the HSV color for the given value.
//------------------------------------------------------------------------------
bool Hsva::colorInterpolate(
      const double value,      // Value
      const double minValue,   // Minimum Value
      const double maxValue,   // Maximum Value 
      const Hsva& minColor,  // Minimum HSV color
      const Hsva& maxColor   // Minimum HSV color
 )
{
   const double p = (value - minValue) / (maxValue - minValue );
   Vec4d deltaColor = maxColor.hsv - minColor.hsv;
   deltaColor[Hsv::HUE] = angle::aepcdDeg(deltaColor[Hsv::HUE]);
   Vec4d newColor = minColor.hsv + deltaColor * p;
   newColor[Hsv::HUE] = angle::aepcdDeg(newColor[Hsv::HUE]);
   if (newColor[Hsv::HUE] < 0) {
      newColor[Hsv::HUE] += 360.0f;
   }
   setHSVA(newColor);
   return true;
}

//------------------------------------------------------------------------------
// setSlotAlpha() -- set the ALPHA value
//------------------------------------------------------------------------------
bool Hsva::setSlotAlpha(const Number* const msg)
{
   if (msg == nullptr) return false;
   double value = msg->getReal();
   bool ok = (value >= 0 && value <= 1);
   if (ok) {
      hsv[ALPHA] = value;
      hsv2rgb(color, hsv);
   }
   else {
      std::cerr << "Hsva::setAlpha: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
   }
   return ok;
}

}
}
