
#include "mixr/simulation/factory.hpp"

#include "mixr/base/IObject.hpp"

#include "mixr/simulation/Simulation.hpp"
#include "mixr/simulation/Station.hpp"

#include <string>

namespace mixr {
namespace simulation {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj {};

    if ( name == Simulation::getFactoryName() ) {
        obj = new Simulation();
    }
    else if ( name == Station::getFactoryName() ) {
        obj = new Station();
    }

    return obj;
}

}
}

