
#include "mixr/models/player/weapon/Missile.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Missile, "Missile")
EMPTY_SLOTTABLE(Missile)
EMPTY_DELETEDATA(Missile)
EMPTY_COPYDATA(Missile)

Missile::Missile()
{
   STANDARD_CONSTRUCTOR()
}

}
}
