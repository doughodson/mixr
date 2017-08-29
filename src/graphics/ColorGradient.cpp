
#include "mixr/graphics/ColorGradient.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(ColorGradient, "ColorGradient")

BEGIN_SLOTTABLE(ColorGradient)
    "colors",      // set colors
END_SLOTTABLE(ColorGradient)

BEGIN_SLOT_MAP(ColorGradient)
    ON_SLOT(1, setSlotColors, base::PairStream)
END_SLOT_MAP()

ColorGradient::ColorGradient()
{
   STANDARD_CONSTRUCTOR()
   // default gives us no colors, but just makes us black
   color[Color::RED] = 0;
   color[Color::GREEN] = 0;
   color[Color::BLUE] = 0;
   color[Color::ALPHA] = getDefaultAlpha();
}

void ColorGradient::copyData(const ColorGradient& org, const bool)
{
    BaseClass::copyData(org);

    setSlotColors(org.myColors);
}

void ColorGradient::deleteData()
{
    if (myColors != nullptr) {
        myColors->unref();
        myColors = nullptr;
    }
}

// set our slot colors via a pairstream
bool ColorGradient::setSlotColors(base::PairStream* const newStream)
{
    if (myColors != nullptr) {
        myColors->unref();
        myColors = nullptr;
    }
    if (newStream != nullptr) {
        myColors = newStream;
        myColors->ref();
    }
    return true;
}

base::Color* ColorGradient::getColorByIdx(const int idx)
{
    base::Color* fCol = nullptr;

    if (myColors != nullptr) {
        base::Pair* pair = myColors->getPosition(idx);
        if (pair != nullptr) {
            fCol = dynamic_cast<base::Color*>(pair->object());
        }
    }
    return fCol;
}

}
}
