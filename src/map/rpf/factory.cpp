
#include "mixr/base/IObject.hpp"

#include "mixr/map/rpf/factory.hpp"
#include "mixr/map/rpf/MapDrawer.hpp"
#include "mixr/map/rpf/CadrgMap.hpp"

#include <string>

namespace mixr {
namespace rpf  {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj {};

    // Map Drawer
    if ( name == MapDrawer::getFactoryName() )     { obj = new MapDrawer(); }

    // CadrgMap
    else if ( name == CadrgMap::getFactoryName() ) { obj = new CadrgMap();  }

    return obj;
}

}
}
