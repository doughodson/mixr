
#include "mixr/interop/rprfom/factory.hpp"
#include "mixr/interop/rprfom/NetIO.hpp"

#include <string>

namespace mixr {
namespace rprfom {

base::Object* formFunc(const std::string& name)
{
    base::Object* obj {};

    if ( name == NetIO::getFactoryName() ) {
        obj = new NetIO();
    }

    return obj;
}

}
}
