
#ifndef __mxrp_gui_glut_factory_H__
#define __mxrp_gui_glut_factory_H__

#include <string>

namespace mxrp {
namespace base { class Object; }
namespace glut {
base::Object* factory(const std::string&);
}
}

#endif
