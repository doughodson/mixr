
#include "mixr/base/colors/Rgba.hpp"

#include "mixr/base/numeric/INumber.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Rgba, "rgba")
EMPTY_COPYDATA(Rgba)
EMPTY_DELETEDATA(Rgba)

BEGIN_SLOTTABLE(Rgba)
    "alpha",    // 1: alpha component,   range(0.0 to 1.0)
END_SLOTTABLE(Rgba)

BEGIN_SLOT_MAP(Rgba)
    ON_SLOT(1, setSlotAlpha, INumber)
END_SLOT_MAP()

Rgba::Rgba(const double r, const double g,
           const double b, const double a) : Rgb(r,g,b)
{
   STANDARD_CONSTRUCTOR()
   color[IColor::ALPHA] = a;
}

Rgba::Rgba()
{
   STANDARD_CONSTRUCTOR()
}

bool Rgba::setSlotAlpha(const INumber* const x)
{
    bool ok{setAlpha(x->asDouble())};
    if (!ok) std::cerr << "Rgba::setAlpha: invalid entry(" << x->asDouble() << "), valid range: 0 to 1" << std::endl;
    return ok;
}

}
}
