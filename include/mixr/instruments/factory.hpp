
#ifndef __mixr_instruments_factory_HPP__
#define __mixr_instruments_factory_HPP__

#include <string>

namespace mixr {
namespace base { class IObject; }
namespace instruments {
base::IObject* factory(const std::string&);
}
}

#endif
