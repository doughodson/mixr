
#include "mixr/interop/dis/factory.hpp"

#include "mixr/base/IObject.hpp"

#include "mixr/interop/dis/NetIO.hpp"
#include "mixr/interop/dis/Ntm.hpp"
#include "mixr/interop/dis/EmissionPduHandler.hpp"

#include <string>

namespace mixr {

namespace dis {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj {};

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
