
#ifndef __mxrp_graphics_factory_H__
#define __mxrp_graphics_factory_H__

#include <string>

namespace mxrp {
namespace base { class Object; }
namespace graphics {
base::Object* factory(const std::string&);
}
}

#endif
