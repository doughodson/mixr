
#ifndef __mixr_models_AbstractAtmosphere_H__
#define __mixr_models_AbstractAtmosphere_H__

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
