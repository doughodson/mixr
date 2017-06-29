
#ifndef __mixr_instruments_factory_H__
#define __mixr_instruments_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace instruments {
base::Object* factory(const std::string&);
}
}

#endif
