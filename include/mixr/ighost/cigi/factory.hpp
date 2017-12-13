
#ifndef __mixr_ighost_cigi_factory_H__
#define __mixr_ighost_cigi_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace cigi {
base::Object* factory(const std::string&);
}
}

#endif
