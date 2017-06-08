
#ifndef __mxrp_models_factory_H__
#define __mxrp_models_factory_H__

#include <string>

namespace mxrp {
namespace base { class Object; }
namespace models {
base::Object* factory(const std::string&);
}
}

#endif
