
#ifndef __mixr_models_common_factory_HPP__
#define __mixr_models_common_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace models {
base::IObject* factory(const std::string&);
}
}

#endif
