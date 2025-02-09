
#ifndef __mixr_graphics_ColorGradient_HPP__
#define __mixr_graphics_ColorGradient_HPP__

#include "mixr/base/colors/IColor.hpp"

namespace mixr {

namespace base { class PairStream; }

namespace graphics {

//------------------------------------------------------------------------------
// Class: ColorGradient
// Description:  List of colors that will return a given color based on the idx.
// This will be used by graphics to create a per-vertex color, which makes a
// gradient.
//------------------------------------------------------------------------------
class ColorGradient : public base::IColor
{
    DECLARE_SUBCLASS(ColorGradient, base::IColor)

public:
    ColorGradient();

    base::IColor* getColorByIdx(const int idx);

private:
    static const int MAX_VALUES = 50;
    base::PairStream* myColors {};

private:
    // slot table helper methods
    bool setSlotColors(base::PairStream* const);
};

}
}

#endif
