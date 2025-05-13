
#ifndef __mixr_models_common_Pilot_HPP__
#define __mixr_models_common_Pilot_HPP__

#include "mixr/models/system/ISystem.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Pilot
// Description: Base pilot model
//
//    Base class for all pilot, autopilot and pilot decision logic models for
//    any mixr Player type, and not just those related to air vehicles.
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.hpp).
//
// Factory name: Pilot
//------------------------------------------------------------------------------
class Pilot : public ISystem
{
   DECLARE_SUBCLASS(Pilot, ISystem)

public:
    Pilot();
};

}
}

#endif
