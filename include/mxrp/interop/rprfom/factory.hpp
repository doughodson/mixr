
#ifndef __mxrp_interop_rprfom_factory_H__
#define __mxrp_interop_rprfom_factory_H__

#include <string>

namespace mxrp {
namespace base { class Object; }
namespace rprfom {
base::Object* factory(const std::string&);
}
}

#endif

