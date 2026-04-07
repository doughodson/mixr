
#include "mixr/models/player/weapon/Missile.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Missile, "Missile")
EMPTY_SLOTTABLE(Missile)
EMPTY_DELETEDATA(Missile)

Missile::Missile()
{
   STANDARD_CONSTRUCTOR()
}

void Missile::copyData(const Missile& org, const bool)
{
    BaseClass::copyData(org);
}

}
}
