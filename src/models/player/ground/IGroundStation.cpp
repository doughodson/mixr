
#include "mixr/models/player/ground/IGroundStation.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IGroundStation, "IGroundStation")
EMPTY_SLOTTABLE(IGroundStation)
EMPTY_DELETEDATA(IGroundStation)
EMPTY_COPYDATA(IGroundStation)

IGroundStation::IGroundStation()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GroundStation");
    setType_old(&generic);
    setType("GroundStation");
}

}
}
