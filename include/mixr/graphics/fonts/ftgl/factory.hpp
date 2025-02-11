
#ifndef __mixr_graphics_fonts_ftgl_factory_HPP__
#define __mixr_graphics_fonts_ftgl_factory_HPP__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace graphics {
namespace ftgl {
base::Object* factory(const std::string&);
}
}
}

#endif
