
#ifndef __mixr_recorder_factory_HPP__
#define __mixr_recorder_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace recorder {
base::IObject* factory(const std::string&);
}
}

#endif
