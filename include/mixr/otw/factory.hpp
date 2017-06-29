
#ifndef __mixr_otw_factory_H__
#define __mixr_otw_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace otw {
base::Object* factory(const std::string&);
}
}

#endif
