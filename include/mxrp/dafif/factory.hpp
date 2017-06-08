
#ifndef __mxrp_dafif_factory_H__
#define __mxrp_dafif_factory_H__

#include <string>

namespace oe {
namespace base { class Object; }
namespace dafif {
base::Object* factory(const std::string&);
}
}

#endif

