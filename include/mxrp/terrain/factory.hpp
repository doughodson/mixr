
#ifndef __mxrp_terrain_factory_H__
#define __mxrp_terrain_factory_H__

#include <string>

namespace mxrp {
namespace base { class Object; }
namespace terrain {
base::Object* factory(const std::string&);
}
}

#endif
