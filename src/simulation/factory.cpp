
#include "mxrp/simulation/factory.hpp"

#include "mxrp/base/Object.hpp"

#include "mxrp/simulation/Simulation.hpp"
#include "mxrp/simulation/Station.hpp"

#include <string>

namespace oe {
namespace simulation {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

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

