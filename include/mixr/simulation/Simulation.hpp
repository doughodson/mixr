
#ifndef __mixr_models_common_Simulation_HPP__
#define __mixr_models_common_Simulation_HPP__

#include "mixr/simulation/ISimulation.hpp"

namespace mixr {
namespace simulation {

//------------------------------------------------------------------------------
// Class: Simulation
//
// Description: Concrete class that defines an executive, a class to manage the execution of a world
//              model that contains a list of players
//
// Factory name: Simulation
//------------------------------------------------------------------------------
class Simulation final: public ISimulation
{
   DECLARE_SUBCLASS(Simulation, ISimulation)

public:
   Simulation();
};

}
}

#endif
