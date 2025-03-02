
#ifndef __mixr_interop_dis_factory_HPP__
#define __mixr_interop_dis_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace dis {
base::IObject* factory(const std::string&);
}
}

#endif
