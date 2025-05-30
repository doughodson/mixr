
#ifndef __mixr_models_IPilot_HPP__
#define __mixr_models_IPilot_HPP__

#include "mixr/models/system/ISystem.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: IPilot
// Description: Interface to pilot models
//
//    Interface class for all pilot, autopilot and pilot decision logic models for
//    any mixr Player type, and not just those related to air vehicles.
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.hpp).
//
// Factory name: IPilot
//------------------------------------------------------------------------------
class IPilot : public ISystem
{
   DECLARE_SUBCLASS(IPilot, ISystem)

public:
    IPilot();
};

}
}

#endif
