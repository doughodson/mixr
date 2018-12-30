
#ifndef __mixr_map_vpf_factory_HPP__
#define __mixr_map_vpf_factory_HPP__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace vpf {
base::Object* factory(const std::string&);
}
}

#endif
