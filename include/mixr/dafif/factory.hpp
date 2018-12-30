
#ifndef __mixr_dafif_factory_HPP__
#define __mixr_dafif_factory_HPP__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace dafif {
base::Object* factory(const std::string&);
}
}

#endif

