
#include "mixr/otw/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/otw/Otm.hpp"

#include "mixr/otw/cigi/OtwCigiCl.hpp"
#include "mixr/otw/cigi/CigiClNetwork.hpp"

#include "mixr/otw/pc/OtwPC.hpp"

#include <string>

namespace mixr {
namespace otw {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    // Common Image Generation Interface (CIGI)
    if ( name == OtwCigiCl::getFactoryName() ) {
        obj = new OtwCigiCl();
    }
    else if ( name == CigiClNetwork::getFactoryName() ) {
        obj = new CigiClNetwork();
    }

    // PC Visual Driver
    else if ( name == OtwPC::getFactoryName() ) {
        obj = new OtwPC();
    }

    else if ( name == Otm::getFactoryName() ) {
        obj = new Otm();
    }

    return obj;
}

}
}
