
#include "mixr/ighost/pov/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/ighost/pov/PovHost.hpp"

#include <string>

namespace mixr {
namespace pov {

base::Object* factory(const std::string& name)
{
    base::Object* obj{};

    if ( name == PovHost::getFactoryName() ) {
        obj = new PovHost();
    }
    return obj;
}

}
}
