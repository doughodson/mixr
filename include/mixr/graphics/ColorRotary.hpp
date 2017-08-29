
#ifndef __mixr_graphics_ColorRotary_H__
#define __mixr_graphics_ColorRotary_H__

#include "mixr/base/colors/Color.hpp"

#include <array>

namespace mixr {
namespace base { class PairStream; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: ColorRotary
//
// Description: Takes a list of colors and breakpoints, and changes colors based
// on the value given to it in comparison with the breakpoints
//
// Factory name: ColorRotary
// Slots:
//      breakcolors: <PairStream>     // sets colors
//      breakpoints: <PairStream>     // sets our breakpoints
//
// Example:
//  ( Graphic
//      color: ( ColorRotary
//                  breakcolors: {
//                      (rgb 1 0 1)
//                      (hsv 0 1 1)
//                  }
//                  breakpoints: {
//                      0
//                      50
//                  }
//  This will remain rgb 1 0 1 until the value hits 50 or more, in which
//  it switches to the next color
//
//  bool ColorRotary::determineColor(const double value)
//      determineColors() - Take our value, and look for a corresponding color
//      and breakpoint
//
//  bool ColorRotary::setSlotColors(PairStream* const newStream)
//       Set our slot colors via a pairstream
//
//  bool ColorRotary::setSlotValues(const PairStream* const newStream)
//       Set our slot values via a pairstream
//
//------------------------------------------------------------------------------
class ColorRotary : public base::Color
{
    DECLARE_SUBCLASS(ColorRotary, base::Color)

public:
    ColorRotary();

    // called to determine a color based on a given value
    virtual bool determineColor(const double value);

private:
    static const unsigned int MAX_VALUES = 50;
    base::PairStream* myColors {};               // our colors (rgba value)
    std::array<double, MAX_VALUES> myValues {};  // our values
    unsigned int numVals {};                     // number of values

private:
    // slot table helper methods
    bool setSlotColors(base::PairStream* const);
    bool setSlotValues(const base::PairStream* const);
};

}
}

#endif
