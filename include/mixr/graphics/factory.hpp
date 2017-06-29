
#ifndef __mixr_graphics_factory_H__
#define __mixr_graphics_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace graphics {
base::Object* factory(const std::string&);
}
}

#endif
