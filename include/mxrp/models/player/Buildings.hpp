
#ifndef __mxrp_models_Building_H__
#define __mxrp_models_Building_H__

#include "mxrp/models/player/Player.hpp"

namespace mxrp {
namespace models {

//------------------------------------------------------------------------------
// Class: Building
// Description: Generic building
// Factory name: Building
//------------------------------------------------------------------------------
class Building : public Player
{
    DECLARE_SUBCLASS(Building, Player)

public:
    Building();

    virtual unsigned int getMajorType() const override;
};

}
}

#endif
