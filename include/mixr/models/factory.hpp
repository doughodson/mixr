
#ifndef __mixr_models_factory_HPP__
#define __mixr_models_factory_HPP__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace models {
base::Object* factory(const std::string&);
}
}

#endif
