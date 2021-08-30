
#ifndef __mixr_ighost_pov_factory_H__
#define __mixr_ighost_pov_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace pov {
base::Object* factory(const std::string&);
}
}

#endif
