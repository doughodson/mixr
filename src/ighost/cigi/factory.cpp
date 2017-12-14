
#include "mixr/ighost/cigi/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/ighost/cigi/TypeMapper.hpp"

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

    // used by CIGI interface
    else if ( name == TypeMapper::getFactoryName() ) {
        obj = new TypeMapper();
    }

    return obj;
}

}
}
