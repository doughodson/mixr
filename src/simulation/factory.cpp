
#include "mixr/simulation/factory.hpp"

#include "mixr/base/IObject.hpp"

#include "mixr/simulation/Simulation.hpp"
#include "mixr/simulation/IStation.hpp"

#include <string>

namespace mixr {
namespace simulation {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj {};

    if ( name == Simulation::getFactoryName() ) {
        obj = new Simulation();
    }
    else if ( name == IStation::getFactoryName() ) {
        obj = new IStation();
    }

    return obj;
}

}
}

