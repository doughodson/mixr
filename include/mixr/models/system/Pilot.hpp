
#ifndef __mixr_models_Pilot_H__
#define __mixr_models_Pilot_H__

#include "mixr/models/system/System.hpp"

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
class Pilot : public System
{
   DECLARE_SUBCLASS(Pilot, System)

public:
    Pilot();
};

}
}

#endif
