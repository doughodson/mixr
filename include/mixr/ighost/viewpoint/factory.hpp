
#ifndef __mixr_ighost_viewpoint_factory_H__
#define __mixr_ighost_viewpoint_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace viewpoint {
base::Object* factory(const std::string&);
}
}

#endif
