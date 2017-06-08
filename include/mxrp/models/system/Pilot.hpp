
#ifndef __mxrp_models_Pilot_H__
#define __mxrp_models_Pilot_H__

#include "mxrp/models/system/System.hpp"

namespace mxrp {
namespace models {

//------------------------------------------------------------------------------
// Class: Pilot
// Description: Base pilot model
//
//    Base class for all pilot, autopilot and pilot decision logic models for
//    any oe Player type, and not just those related to air vehicles.
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.h).
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
