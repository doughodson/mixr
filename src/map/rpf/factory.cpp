
#include "mixr/base/Object.hpp"

#include "mixr/map/rpf/factory.hpp"
#include "mixr/map/rpf/MapDrawer.hpp"
#include "mixr/map/rpf/CadrgMap.hpp"

#include <string>

namespace mixr {
namespace rpf  {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    // Map Drawer
    if ( name == MapDrawer::getFactoryName() )     { obj = new MapDrawer(); }

    // CadrgMap
    else if ( name == CadrgMap::getFactoryName() ) { obj = new CadrgMap();  }

    return obj;
}

}
}
