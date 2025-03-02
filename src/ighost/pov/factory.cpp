
#include "mixr/ighost/pov/factory.hpp"

#include "mixr/base/IObject.hpp"

#include "mixr/ighost/pov/PovHost.hpp"

#include <string>

namespace mixr {
namespace pov {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj{};

    if ( name == PovHost::getFactoryName() ) {
        obj = new PovHost();
    }
    return obj;
}

}
}
