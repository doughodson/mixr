
#include "mixr/simulation/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/simulation/Simulation.hpp"
#include "mixr/simulation/Station.hpp"

#include <string>

namespace mixr {
namespace simulation {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    if ( name == Simulation::getFactoryName() )   { obj = new Simulation(); }
    else if ( name == Station::getFactoryName() ) { obj = new Station();    }

    return obj;
}

}
}

