
#ifndef __mixr_ui_glut_factory_HPP__
#define __mixr_ui_glut_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace glut {
base::IObject* factory(const std::string&);
}
}

#endif
