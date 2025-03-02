
#ifndef __mixr_terrain_factory_HPP__
#define __mixr_terrain_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace terrain {
base::IObject* factory(const std::string&);
}
}

#endif
