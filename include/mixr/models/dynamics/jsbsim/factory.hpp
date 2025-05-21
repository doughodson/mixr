
#ifndef __mixr_models_DynamicsJSBSim_factory_HPP__
#define __mixr_models_DynamicsJSBSim_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace models {
namespace jsbsim {
base::IObject* factory(const std::string&);
}
}
}

#endif
