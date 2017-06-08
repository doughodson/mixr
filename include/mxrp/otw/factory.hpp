
#ifndef __mxrp_otw_factory_H__
#define __mxrp_otw_factory_H__

#include <string>

namespace mxrp {
namespace base { class Object; }
namespace otw {
base::Object* factory(const std::string&);
}
}

#endif
