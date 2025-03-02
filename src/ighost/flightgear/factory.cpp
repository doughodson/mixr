
#include "mixr/ighost/flightgear/factory.hpp"

#include "mixr/base/IObject.hpp"

#include "mixr/ighost/flightgear/FlightGearHost.hpp"

#include <string>

namespace mixr {
namespace flightgear {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj{};

    if ( name == FlightGearHost::getFactoryName() ) {
        obj = new FlightGearHost();
    }
    return obj;
}

}
}
