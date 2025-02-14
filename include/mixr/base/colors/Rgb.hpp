
#ifndef __mixr_base_colors_Rgb_HPP__
#define __mixr_base_colors_Rgb_HPP__

#include "mixr/base/colors/IColor.hpp"

namespace mixr {
namespace base {
class INumber;

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
//     red   <INumber>  ! Red component (0.0 to 1.0) (default: 0)
//     green <INumber>  ! Green component (0.0 to 1.0) (default: 0)
//     blue  <INumber>  ! Blue component (0.0 to 1.0) (default: 0)
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
    bool setSlotRed(const INumber* const);
    bool setSlotGreen(const INumber* const);
    bool setSlotBlue(const INumber* const);
};

}
}

#endif
