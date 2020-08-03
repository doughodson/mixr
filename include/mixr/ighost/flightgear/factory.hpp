
#ifndef __mixr_ighost_flightgear_factory_HPP__
#define __mixr_ighost_flightgear_factory_HPP__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace flightgear {
base::Object* factory(const std::string&);
}
}

#endif
