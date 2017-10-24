
#include "mixr/instruments/gauges/GaugeSlider.hpp"
#include <iostream>

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(GaugeSlider, "GaugeSlider")
EMPTY_SLOTTABLE(GaugeSlider)
EMPTY_DELETEDATA(GaugeSlider)

GaugeSlider::GaugeSlider()
{
    STANDARD_CONSTRUCTOR()
    setDrawMe(false);
}

void GaugeSlider::copyData(const GaugeSlider& org, const bool)
{
    BaseClass::copyData(org);
    sliderPos = org.sliderPos;
}

//------------------------------------------------------------------------------
// draw() -- draws the object(s)
//------------------------------------------------------------------------------
void GaugeSlider::draw()
{
    sliderPos = getInstValue();
    bool vertical = isVertical();
    lcSaveMatrix();
    if (vertical) {
        lcTranslate(0, sliderPos);
    } else {
        lcTranslate(sliderPos, 0);
    }

    BaseClass::draw();

    lcRestoreMatrix();
}

}
}
