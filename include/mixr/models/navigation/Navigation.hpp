
#ifndef __mixr_models_common_Navigation_HPP__
#define __mixr_models_common_Navigation_HPP__

#include "mixr/models/navigation/INavigation.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Navigation
// Description: Interface to a navigation system ...
//              Maintains present position; routes; steering cues;
//              NAV subsystems (INS, GPS); NAVAIDS (TACAN, ILS)
//
//    This class is one of the "top level" systems attached to a Player
//    class (see Player.hpp).
//
// Factory name: Navigation
//------------------------------------------------------------------------------
class Navigation final: public INavigation
{
   DECLARE_SUBCLASS(Navigation, INavigation)

public:
   Navigation();
};

}
}

#endif
