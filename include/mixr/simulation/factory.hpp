
#ifndef __mixr_simulation_factory_H__
#define __mixr_simulation_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace simulation {
base::Object* factory(const std::string&);
}
}

#endif
