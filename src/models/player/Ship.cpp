
#include "mixr/models/player/Ship.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/osg/Matrixd"
#include "mixr/base/units/Angles.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Ship, "Ship")
EMPTY_SLOTTABLE(Ship)

Ship::Ship()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericShip");
    setType(&generic);
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
