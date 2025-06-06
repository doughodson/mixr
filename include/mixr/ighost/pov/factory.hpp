
#ifndef __mixr_ighost_pov_factory_H__
#define __mixr_ighost_pov_factory_H__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace ighost {
namespace pov {
base::IObject* factory(const std::string&);
}
}
}

#endif
