
#ifndef __mixr_models_common_IAtmosphere_HPP__
#define __mixr_models_common_IAtmosphere_HPP__

#include "mixr/base/Component.hpp"

namespace mixr {
namespace models {

class IAtmosphere : public base::Component
{
   DECLARE_SUBCLASS(IAtmosphere, base::Component)

public:
   IAtmosphere();
};

}
}

#endif
