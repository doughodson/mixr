
#include "mixr/models/player/Ship.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Ship, "Ship")
EMPTY_SLOTTABLE(Ship)

Ship::Ship()
{
    STANDARD_CONSTRUCTOR()
    setType("GenericShip");
}

void Ship::copyData(const Ship& org, const bool)
{
    BaseClass::copyData(org);
}

void Ship::deleteData()
{
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int Ship::getMajorType() const
{
    return SHIP;
}

}
}
