
#include "mixr/ighost/viewpoint/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/ighost/viewpoint/Viewpoint.hpp"

#include <string>

namespace mixr {
namespace viewpoint {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    if ( name == Viewpoint::getFactoryName() ) {
        obj = new Viewpoint();
    }
    return obj;
}

}
}
