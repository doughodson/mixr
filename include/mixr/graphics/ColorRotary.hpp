
#ifndef __mixr_graphics_ColorRotary_HPP__
#define __mixr_graphics_ColorRotary_HPP__

#include "mixr/base/colors/IColor.hpp"

#include <array>

namespace mixr {
namespace base { class IPairStream; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: ColorRotary
//
// Description: Takes a list of colors and breakpoints, and changes colors based
// on the value given to it in comparison with the breakpoints
//
// Factory name: ColorRotary
// Slots:
//      breakcolors: <IPairStream>     // sets colors
//      breakpoints: <IPairStream>     // sets our breakpoints
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
class ColorRotary : public base::IColor
{
    DECLARE_SUBCLASS(ColorRotary, base::IColor)

public:
    ColorRotary();

    // called to determine a color based on a given value
    virtual bool determineColor(const double value);

private:
    static const unsigned int MAX_VALUES = 50;
    base::IPairStream* myColors {};              // our colors (rgba value)
    std::array<double, MAX_VALUES> myValues {};  // our values
    unsigned int numVals {};                     // number of values

private:
    // slot table helper methods
    bool setSlotColors(base::IPairStream* const);
    bool setSlotValues(const base::IPairStream* const);
};

}
}

#endif
