
#ifndef __mixr_models_jsbsim_factory_HPP__
#define __mixr_models_jsbsim_factory_HPP__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace jsbsim {
base::Object* factory(const std::string&);
}
}

#endif
