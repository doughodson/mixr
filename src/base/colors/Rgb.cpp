
#include "mixr/base/colors/Rgb.hpp"

#include "mixr/base/numeric/Float.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Rgb, "rgb")
EMPTY_DELETEDATA(Rgb)

BEGIN_SLOTTABLE(Rgb)
    "red",      // 1: red component,   range(0.0f .. 1.0f)
    "green",    // 2: green component, range(0.0f .. 1.0f)
    "blue",     // 3: blue component,  range(0.0f .. 1.0f)
END_SLOTTABLE(Rgb)

BEGIN_SLOT_MAP(Rgb)
    ON_SLOT(1, setSlotRed,   Number)
    ON_SLOT(2, setSlotGreen, Number)
    ON_SLOT(3, setSlotBlue,  Number)
END_SLOT_MAP()

Rgb::Rgb(const double r, const double g, const double b)
{
   STANDARD_CONSTRUCTOR()
   color[Color::RED]   = r;
   color[Color::GREEN] = g;
   color[Color::BLUE]  = b;
   color[Color::ALPHA] = getDefaultAlpha();
}

Rgb::Rgb()
{
   STANDARD_CONSTRUCTOR()
}

void Rgb::copyData(const Rgb& org, const bool)
{
   BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool Rgb::setSlotRed(const Number* const msg)
{
    const double value{msg->getReal()};
    const bool ok{setRed( value )};
    if (!ok) {
        std::cerr << "Rgb::setRed: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

bool Rgb::setSlotGreen(const Number* const msg)
{
    const double value{msg->getReal()};
    const bool ok{setGreen( value )};
    if (!ok) {
        std::cerr << "Rgb::setGreen: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

bool Rgb::setSlotBlue(const Number* const msg)
{
    const double value{msg->getReal()};
    const bool ok{setBlue( value )};
    if (!ok) {
        std::cerr << "Rgb::setBlue: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

}
}
