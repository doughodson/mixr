
#ifndef __mxrp_graphics_ColorGradient_H__
#define __mxrp_graphics_ColorGradient_H__

#include "mxrp/base/Color.hpp"

namespace mxrp {

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

protected:
    bool setSlotColors(base::PairStream* const newStream);

private:
    static const int MAX_VALUES = 50;
    base::PairStream* myColors {};
};

}
}

#endif
