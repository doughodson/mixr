
#ifndef __mxrp_iodevice_factory_H__
#define __mxrp_iodevice_factory_H__

#include <string>

namespace mxrp {
namespace base { class Object; }
namespace iodevice {
base::Object* factory(const std::string&);
}
}

#endif
