
#include "mixr/models/player/ground/Tank.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Tank, "Tank")
EMPTY_SLOTTABLE(Tank)
EMPTY_DELETEDATA(Tank)
EMPTY_COPYDATA(Tank)

Tank::Tank()
{
    STANDARD_CONSTRUCTOR()
    setType("GenericTank");
}

}
}
