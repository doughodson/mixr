
#include "mxrp/base/Object.hpp"

#include "mxrp/map/rpf/factory.hpp"
#include "mxrp/map/rpf/MapDrawer.hpp"
#include "mxrp/map/rpf/CadrgMap.hpp"

#include <string>

namespace mxrp {
namespace rpf  {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // Map Drawer
    if ( name == MapDrawer::getFactoryName() ) {
        obj = new MapDrawer();
    }
    // CadrgMap
    else if ( name == CadrgMap::getFactoryName() ) {
        obj = new CadrgMap();
    }

    return obj;
}

}
}
