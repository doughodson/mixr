
#ifndef __mixr_base_factory_HPP__
#define __mixr_base_factory_HPP__

#include <string>

namespace mixr {
namespace base {

class IObject;
IObject* factory(const std::string&);
}
}

#endif

