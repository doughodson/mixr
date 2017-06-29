
#ifndef __mixr_terrain_factory_H__
#define __mixr_terrain_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace terrain {
base::Object* factory(const std::string&);
}
}

#endif
