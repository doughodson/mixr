
#include "mixr/instruments/dials/DialPointer.hpp"

#include "mixr/base/units/angle_utils.hpp"

namespace mixr {
namespace instruments {

IMPLEMENT_EMPTY_SLOTTABLE_SUBCLASS(DialPointer, "DialPointer")
EMPTY_COPYDATA(DialPointer)
EMPTY_DELETEDATA(DialPointer)

DialPointer::DialPointer()
{
    STANDARD_CONSTRUCTOR()
}

void DialPointer::draw()
{
    double pointerPos = getInstValue();
    double startAngle = getStartAngle();
    lcSaveMatrix();
        lcRotate((-pointerPos - startAngle) * static_cast<double>(base::angle::D2RCC));
        graphics::Graphic::draw();
    lcRestoreMatrix();
}

}
}
