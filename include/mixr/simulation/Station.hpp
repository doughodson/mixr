
#ifndef __mixr_models_common_Station_HPP__
#define __mixr_models_common_Station_HPP__

#include "mixr/simulation/IStation.hpp"

namespace mixr {
namespace simulation {

//------------------------------------------------------------------------------
// Class: Station
//
// Description: Concrete class that defines a top level container class with support for the
//               simulation model, controls & displays, network interfaces
//               and image generator systems.  Also, contains default
//               support for the time-critical and network threads.
//
// Factory name: Station
//------------------------------------------------------------------------------
class Station final: public IStation
{
   DECLARE_SUBCLASS(Station, IStation)

public:
   Station();
};

}
}

#endif
