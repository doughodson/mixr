
#ifndef __mixr_linkage_factory_H__
#define __mixr_linkage_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace linkage {
base::Object* factory(const std::string&);
}
}

#endif
