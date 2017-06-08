
#include "mxrp/interop/dis/factory.hpp"

#include "mxrp/base/Object.hpp"

#include "mxrp/interop/dis/NetIO.hpp"
#include "mxrp/interop/dis/Ntm.hpp"
#include "mxrp/interop/dis/EmissionPduHandler.hpp"

#include <string>

namespace mxrp {

namespace dis {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == NetIO::getFactoryName() ) {
        obj = new NetIO();
    }
    else if ( name == Ntm::getFactoryName() ) {
        obj = new Ntm();
    }
    else if ( name == EmissionPduHandler::getFactoryName() ) {
        obj = new EmissionPduHandler();
    }

    return obj;
}

}
}
