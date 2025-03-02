
#ifndef __mixr_dafif_factory_HPP__
#define __mixr_dafif_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace dafif {
base::IObject* factory(const std::string&);
}
}

#endif

