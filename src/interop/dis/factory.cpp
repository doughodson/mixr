
#include "mixr/interop/dis/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/interop/dis/NetIO.hpp"
#include "mixr/interop/dis/Ntm.hpp"
#include "mixr/interop/dis/EmissionPduHandler.hpp"

#include <string>

namespace mixr {

namespace dis {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

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
