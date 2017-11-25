
#ifndef __mixr_iolinkage_factory_H__
#define __mixr_iolinkage_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace iolinkage {
base::Object* factory(const std::string&);
}
}

#endif
