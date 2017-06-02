
#include "mxrp/dafif/factory.hpp"

#include "mxrp/base/Object.hpp"

#include "mxrp/dafif/AirportLoader.hpp"
#include "mxrp/dafif/NavaidLoader.hpp"
#include "mxrp/dafif/WaypointLoader.hpp"

#include <string>

namespace oe {
namespace dafif {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

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

