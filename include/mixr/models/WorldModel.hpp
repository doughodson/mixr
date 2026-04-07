
#ifndef __mixr_models_common_WorldModel_HPP__
#define __mixr_models_common_WorldModel_HPP__

#include "mixr/models/IWorldModel.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: WorldModel
//
// Description: Concrete model of the spatial world in terms of earth model, coordinates,
//              lat/long, etc.
//------------------------------------------------------------------------------
class WorldModel final: public IWorldModel
{
    DECLARE_SUBCLASS(WorldModel, IWorldModel)

public:
    WorldModel();
};

}
}

#endif
