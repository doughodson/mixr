
#include "mixr/models/player/ground/Artillery.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Artillery, "Artillery")
EMPTY_SLOTTABLE(Artillery)
EMPTY_DELETEDATA(Artillery)
EMPTY_COPYDATA(Artillery)

Artillery::Artillery()
{
    STANDARD_CONSTRUCTOR()
    static base::String generic("GenericArtillery");
    setType(&generic);
}

}
}
