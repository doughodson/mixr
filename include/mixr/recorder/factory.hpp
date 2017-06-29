
#ifndef __mixr_recorder_factory_H__
#define __mixr_recorder_factory_H__

#include <string>

namespace mixr {
namespace base { class Object; }
namespace recorder {
base::Object* factory(const std::string&);
}
}

#endif
