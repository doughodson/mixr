
#include "mixr/models/player/ground/GroundStationUav.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(GroundStationUav, "GroundStationUav")
EMPTY_SLOTTABLE(GroundStationUav)
EMPTY_DELETEDATA(GroundStationUav)
EMPTY_COPYDATA(GroundStationUav)

GroundStationUav::GroundStationUav()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GroundStationUav");
    setType(&generic);
}

}
}
