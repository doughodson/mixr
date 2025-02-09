
#include "mixr/base/colors/Rgb.hpp"

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Rgb, "Rgb")
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
   color[IColor::RED]   = r;
   color[IColor::GREEN] = g;
   color[IColor::BLUE]  = b;
   color[IColor::ALPHA] = getDefaultAlpha();
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
bool Rgb::setSlotRed(const Number* const x)
{
    const bool ok{setRed(x->asDouble())};
    if (!ok) {
        std::cerr << "Rgb::setRed: invalid entry(" << x->asDouble() << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

bool Rgb::setSlotGreen(const Number* const x)
{
    const bool ok{setGreen(x->asDouble())};
    if (!ok) {
        std::cerr << "Rgb::setGreen: invalid entry(" << x->asDouble() << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

bool Rgb::setSlotBlue(const Number* const x)
{
    const bool ok{setBlue(x->asDouble())};
    if (!ok) {
        std::cerr << "Rgb::setBlue: invalid entry(" << x->asDouble() << "), valid range: 0 to 1" << std::endl;
    }
    return ok;
}

}
}
