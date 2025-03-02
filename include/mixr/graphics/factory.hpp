
#ifndef __mixr_graphics_factory_HPP__
#define __mixr_graphics_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace graphics {
base::IObject* factory(const std::string&);
}
}

#endif
