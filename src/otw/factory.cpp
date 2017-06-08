
#include "mxrp/otw/factory.hpp"

#include "mxrp/base/Object.hpp"

#include "mxrp/otw/Otm.hpp"

#include "mxrp/otw/cigi/OtwCigiCl.hpp"
#include "mxrp/otw/cigi/CigiClNetwork.hpp"

#include "mxrp/otw/pc/OtwPC.hpp"

#include <string>

namespace mxrp {
namespace otw {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

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
