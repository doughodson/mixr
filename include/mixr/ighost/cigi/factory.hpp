
#ifndef __mixr_ighost_cigi3_factory_HPP__
#define __mixr_ighost_cigi3_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace ighost {
namespace cigi {
base::IObject* factory(const std::string&);
}
}
}

#endif
