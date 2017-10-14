
#ifndef __mixr_base_colors_Rgb_H__
#define __mixr_base_colors_Rgb_H__

#include "mixr/base/colors/Color.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  Rgb
//
// Description:  Defines a color by Red, Green, and Blue
//
// Factory name: rgb
// Slots:
//     red   <Number>  ! Red component (0.0 to 1.0) (default: 0)
//     green <Number>  ! Green component (0.0 to 1.0) (default: 0)
//     blue  <Number>  ! Blue component (0.0 to 1.0) (default: 0)
//
// Events: None
//
//
// Public methods: Base class public methods, plus ...
//
//      Rgb(const double r, const double g, const double b)
//          Special constructor what initializes the object to r, g, and b
//
//------------------------------------------------------------------------------
class Rgb : public Color
{
    DECLARE_SUBCLASS(Rgb, Color)

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
