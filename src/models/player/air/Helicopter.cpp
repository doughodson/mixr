
#include "mixr/models/player/air/Helicopter.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Helicopter, "Helicopter")
EMPTY_SLOTTABLE(Helicopter)
EMPTY_DELETEDATA(Helicopter)
EMPTY_COPYDATA(Helicopter)

Helicopter::Helicopter()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericHelicopter");
    setType(&generic);
}

}
}
