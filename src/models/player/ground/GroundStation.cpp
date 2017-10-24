
#include "mixr/models/player/ground/GroundStation.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(GroundStation, "GroundStation")
EMPTY_SLOTTABLE(GroundStation)
EMPTY_DELETEDATA(GroundStation)
EMPTY_COPYDATA(GroundStation)

GroundStation::GroundStation()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GroundStation");
    setType(&generic);
}

}
}
