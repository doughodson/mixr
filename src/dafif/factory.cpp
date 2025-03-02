
#include "mixr/dafif/factory.hpp"

#include "mixr/base/IObject.hpp"

#include "mixr/dafif/loaders/AirportLoader.hpp"
#include "mixr/dafif/loaders/NavaidLoader.hpp"
#include "mixr/dafif/loaders/WaypointLoader.hpp"

#include <string>

namespace mixr {
namespace dafif {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj {};

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

