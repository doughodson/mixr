
#include "mixr/ighost/flightgear/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/ighost/flightgear/FlightGearHost.hpp"

#include <string>

namespace mixr {
namespace flightgear {

base::Object* factory(const std::string& name)
{
    base::Object* obj{};

    if ( name == FlightGearHost::getFactoryName() ) {
        obj = new FlightGearHost();
    }
    return obj;
}

}
}
