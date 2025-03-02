
#ifndef __mixr_map_rpf_factory_HPP__
#define __mixr_map_rpf_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace rpf {
base::IObject* factory(const std::string&);
}
}

#endif
