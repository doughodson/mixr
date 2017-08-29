
#ifndef __mixr_graphics_ColorGradient_H__
#define __mixr_graphics_ColorGradient_H__

#include "mixr/base/colors/Color.hpp"

namespace mixr {

namespace base { class PairStream; }

namespace graphics {

//------------------------------------------------------------------------------
// Class: ColorGradient
// Description:  List of colors that will return a given color based on the idx.
// This will be used by graphics to create a per-vertex color, which makes a
// gradient.
//------------------------------------------------------------------------------
class ColorGradient : public base::Color
{
    DECLARE_SUBCLASS(ColorGradient, base::Color)

public:
    ColorGradient();

    base::Color* getColorByIdx(const int idx);

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
