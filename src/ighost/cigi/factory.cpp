
#include "mixr/ighost/cigi/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/ighost/cigi/Otm.hpp"

#include "mixr/ighost/cigi/Cigi.hpp"
#include "mixr/ighost/cigi/session/HostSession.hpp"

#include <string>

namespace mixr {
namespace cigi {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    // Common Image Generation Interface (CIGI)
    if ( name == Cigi::getFactoryName() ) {
        obj = new Cigi();
    }
    else if ( name == HostSession::getFactoryName() ) {
        obj = new HostSession();
    }

    // used by CIGI interface
    else if ( name == Otm::getFactoryName() ) {
        obj = new Otm();
    }

    return obj;
}

}
}
