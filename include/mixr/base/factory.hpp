
#ifndef __mixr_base_factory_H__
#define __mixr_base_factory_H__

#include <string>

namespace mixr {
namespace base {

class Object;
Object* factory(const std::string&);
}
}

#endif

