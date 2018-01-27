
#include "mixr/base/units/volume/Volume.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Volume, "AbstractVolume")
EMPTY_SLOTTABLE(Volume)
EMPTY_DELETEDATA(Volume)

Volume::Volume()
{
    STANDARD_CONSTRUCTOR()
}

Volume::Volume(const double value) : Unit(value)
{
    STANDARD_CONSTRUCTOR()
}

void Volume::copyData(const Volume& org, const bool)
{
   BaseClass::copyData(org);
   setValue(fromVolume(org.toVolume()));
}

}
}

