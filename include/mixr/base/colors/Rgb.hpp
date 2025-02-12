
#ifndef __mixr_base_colors_Rgb_HPP__
#define __mixr_base_colors_Rgb_HPP__

#include "mixr/base/colors/IColor.hpp"

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Rgb
//
// Description: Defines a color by Red, Green, and Blue
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: rgb
//
// Slots:
//     red   <Number>  ! Red component (0.0 to 1.0) (default: 0)
//     green <Number>  ! Green component (0.0 to 1.0) (default: 0)
//     blue  <Number>  ! Blue component (0.0 to 1.0) (default: 0)
//------------------------------------------------------------------------------
// Events: None
//------------------------------------------------------------------------------
class Rgb : public IColor
{
    DECLARE_SUBCLASS(Rgb, IColor)

public:
    Rgb(const double r, const double g, const double b);
    Rgb();

private:
    // slot table helper methods
    bool setSlotRed(const Number* const);
    bool setSlotGreen(const Number* const);
    bool setSlotBlue(const Number* const);
};

}
}

#endif
