
#ifndef __mixr_simulation_factory_HPP__
#define __mixr_simulation_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace simulation {
base::IObject* factory(const std::string&);
}
}

#endif
