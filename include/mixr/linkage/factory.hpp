
#ifndef __mixr_linkage_factory_HPP__
#define __mixr_linkage_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace linkage {
base::IObject* factory(const std::string&);
}
}

#endif
