
#include "mixr/models/WorldModel.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(WorldModel, "WorldModel")
EMPTY_SLOTTABLE(WorldModel)
EMPTY_DELETEDATA(WorldModel)
EMPTY_COPYDATA(WorldModel)

WorldModel::WorldModel()
{
    STANDARD_CONSTRUCTOR()
}

}
}
