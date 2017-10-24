
#include "mixr/models/player/effect/Decoy.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Decoy, "Decoy")
EMPTY_SLOTTABLE(Decoy)
EMPTY_COPYDATA(Decoy)
EMPTY_DELETEDATA(Decoy)

// Weapon data for general bomb
int Decoy::getCategory() const                { return GRAVITY; }
const char* Decoy::getDescription() const     { return "Generic Decoy"; }
const char* Decoy::getNickname() const        { return "Decoy"; }

Decoy::Decoy()
{
    STANDARD_CONSTRUCTOR()

    static base::String generic("Decoy");
    setType(&generic);
}

}
}
