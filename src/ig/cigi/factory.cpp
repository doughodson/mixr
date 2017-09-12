
#include "mixr/ig/cigi/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/ig/common/Otm.hpp"

#include "mixr/ig/cigi/OtwCigiCl.hpp"
#include "mixr/ig/cigi/CigiClNetwork.hpp"

#include <string>

namespace mixr {
namespace cigi {

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

    // used by CIGI interface
    else if ( name == ::mixr::ig::Otm::getFactoryName() ) {
        obj = new ::mixr::ig::Otm();
    }

    return obj;
}

}
}
