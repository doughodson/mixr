
#include "mixr/models/player/effect/Chaff.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Chaff, "Chaff")
EMPTY_SLOTTABLE(Chaff)
EMPTY_COPYDATA(Chaff)
EMPTY_DELETEDATA(Chaff)

// Weapon data for general bomb
int Chaff::getCategory() const                { return GRAVITY; }
const char* Chaff::getDescription() const     { return "Generic Chaff"; }
const char* Chaff::getNickname() const        { return "Chaff"; }

Chaff::Chaff()
{
    STANDARD_CONSTRUCTOR()

    static base::String generic("Chaff");
    setType(&generic);
}

}
}
