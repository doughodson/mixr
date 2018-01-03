
#include "mixr/ighost/cigi/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/ighost/cigi/Player2CigiMap.hpp"

#include "mixr/ighost/cigi/CigiHost.hpp"
#include "mixr/ighost/cigi/session/HostSession.hpp"

#include <string>

namespace mixr {
namespace cigi {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    // Common Image Generation Interface (CIGI)
    if ( name == CigiHost::getFactoryName() ) {
        obj = new CigiHost();
    }
    else if ( name == HostSession::getFactoryName() ) {
        obj = new HostSession();
    }

    // Player to CIGI entity type map
    else if ( name == Player2CigiMap::getFactoryName() ) {
        obj = new Player2CigiMap();
    }

    return obj;
}

}
}
