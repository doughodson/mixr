
#include "mixr/interop/hla/rprfom/factory.hpp"
#include "mixr/interop/hla/rprfom/NetIO.hpp"

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
