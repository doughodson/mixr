
#ifndef __mixr_models_common_AbstractAtmosphere_HPP__
#define __mixr_models_common_AbstractAtmosphere_HPP__

#include "mixr/base/Component.hpp"

namespace mixr {
namespace models {

class AbstractAtmosphere : public base::Component
{
   DECLARE_SUBCLASS(AbstractAtmosphere, base::Component)

public:
   AbstractAtmosphere();
};

}
}

#endif
