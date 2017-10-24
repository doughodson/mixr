
#include "mixr/models/player/ground/GroundStationRadar.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(GroundStationRadar, "GroundStationRadar")
EMPTY_SLOTTABLE(GroundStationRadar)
EMPTY_DELETEDATA(GroundStationRadar)
EMPTY_COPYDATA(GroundStationRadar)

GroundStationRadar::GroundStationRadar()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GroundStationRadar");
    setType(&generic);
}

}
}
