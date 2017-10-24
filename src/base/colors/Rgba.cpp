
#include "mixr/base/colors/Rgba.hpp"

#include "mixr/base/numeric/Float.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Rgba, "rgba")
EMPTY_COPYDATA(Rgba)
EMPTY_DELETEDATA(Rgba)

BEGIN_SLOTTABLE(Rgba)
    "alpha",    // 1: alpha component,   range(0.0 to 1.0)
END_SLOTTABLE(Rgba)

BEGIN_SLOT_MAP(Rgba)
    ON_SLOT(1, setSlotAlpha, Number)
END_SLOT_MAP()

Rgba::Rgba(const double r, const double g,
           const double b, const double a) : Rgb(r,g,b)
{
   STANDARD_CONSTRUCTOR()
   color[Color::ALPHA] = a;
}

Rgba::Rgba()
{
   STANDARD_CONSTRUCTOR()
}

bool Rgba::setSlotAlpha(const Number* const msg)
{
    double value{msg->getReal()};
    bool ok{setAlpha( value )};
    if (!ok) std::cerr << "Rgba::setAlpha: invalid entry(" << value << "), valid range: 0 to 1" << std::endl;
    return ok;
}

}
}
