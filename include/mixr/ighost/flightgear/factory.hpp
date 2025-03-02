
#ifndef __mixr_ighost_flightgear_factory_HPP__
#define __mixr_ighost_flightgear_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace flightgear {
base::IObject* factory(const std::string&);
}
}

#endif
