
#include "mixr/models/sensor/Tws.hpp"
#include <iostream>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Tws, "Tws")
EMPTY_SLOTTABLE(Tws)
EMPTY_DELETEDATA(Tws)

Tws::Tws()
{
    STANDARD_CONSTRUCTOR()
}

void Tws::copyData(const Tws& org, const bool)
{
    BaseClass::copyData(org);
}

}
}
