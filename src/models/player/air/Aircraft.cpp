
#include "mixr/models/player/air/Aircraft.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Aircraft, "Aircraft")
EMPTY_SLOTTABLE(Aircraft)
EMPTY_DELETEDATA(Aircraft)
EMPTY_COPYDATA(Aircraft)

Aircraft::Aircraft()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericAircraft");
    setType(&generic);
}

}
}
