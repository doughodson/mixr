
#include "mixr/ig/viewpoint/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/ig/viewpoint/OtwPC.hpp"

#include <string>

namespace mixr {
namespace viewpoint {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    if ( name == OtwPC::getFactoryName() ) {
        obj = new OtwPC();
    }
    return obj;
}

}
}
