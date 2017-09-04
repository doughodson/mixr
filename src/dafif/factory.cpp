
#include "mixr/dafif/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/dafif/AirportLoader.hpp"
#include "mixr/dafif/NavaidLoader.hpp"
#include "mixr/dafif/WaypointLoader.hpp"

#include <string>

namespace mixr {
namespace dafif {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    if ( name == AirportLoader::getFactoryName() ) {
        obj = new AirportLoader();
    }
    else if ( name == NavaidLoader::getFactoryName() ) {
        obj = new NavaidLoader();
    }
    else if ( name == WaypointLoader::getFactoryName() ) {
        obj = new WaypointLoader();
    }

    return obj;
}

}
}

