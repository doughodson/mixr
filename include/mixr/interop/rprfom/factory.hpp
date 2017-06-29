
#ifndef __mixr_interop_rprfom_factory_H__
#define __mixr_interop_rprfom_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace rprfom {
base::Object* factory(const std::string&);
}
}

#endif

