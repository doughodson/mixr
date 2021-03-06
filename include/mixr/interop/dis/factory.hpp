
#ifndef __mixr_interop_dis_factory_HPP__
#define __mixr_interop_dis_factory_HPP__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace dis {
base::Object* factory(const std::string&);
}
}

#endif
