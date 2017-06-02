
#include "mxrp/interop/rprfom/factory.hpp"
#include "mxrp/interop/rprfom/NetIO.hpp"

#include <string>

namespace oe {
namespace rprfom {

base::Object* formFunc(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == NetIO::getFactoryName() ) {
        obj = new NetIO();
    }

    return obj;
}

}
}
