
#include "mixr/graphics/ColorGradient.hpp"
#include "mixr/base/IPairStream.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(ColorGradient, "ColorGradient")

BEGIN_SLOTTABLE(ColorGradient)
    "colors",      // set colors
END_SLOTTABLE(ColorGradient)

BEGIN_SLOT_MAP(ColorGradient)
    ON_SLOT(1, setSlotColors, base::IPairStream)
END_SLOT_MAP()

ColorGradient::ColorGradient()
{
   STANDARD_CONSTRUCTOR()
   // default gives us no colors, but just makes us black
   color[IColor::RED] = 0;
   color[IColor::GREEN] = 0;
   color[IColor::BLUE] = 0;
   color[IColor::ALPHA] = getDefaultAlpha();
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

base::IColor* ColorGradient::getColorByIdx(const int idx)
{
    base::IColor* fCol{};

    if (myColors != nullptr) {
        base::Pair* pair = myColors->getPosition(idx);
        if (pair != nullptr) {
            fCol = dynamic_cast<base::IColor*>(pair->object());
        }
    }
    return fCol;
}

// set our slot colors via a pairstream
bool ColorGradient::setSlotColors(base::IPairStream* const x)
{
    if (myColors != nullptr) {
        myColors->unref();
        myColors = nullptr;
    }
    myColors = x;
    myColors->ref();
    return true;
}

}
}
