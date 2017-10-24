
#include "mixr/models/player/effect/Flare.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Flare, "Flare")
EMPTY_SLOTTABLE(Flare)
EMPTY_COPYDATA(Flare)
EMPTY_DELETEDATA(Flare)

// Weapon data for general bomb
int Flare::getCategory() const                { return GRAVITY; }
const char* Flare::getDescription() const     { return "Generic Flare"; }
const char* Flare::getNickname() const        { return "Flare"; }

Flare::Flare()
{
    STANDARD_CONSTRUCTOR()

    static base::String generic("Flare");
    setType(&generic);
}

}
}
