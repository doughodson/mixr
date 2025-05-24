
#ifndef __mixr_models_IAtmosphere_HPP__
#define __mixr_models_IAtmosphere_HPP__

#include "mixr/base/IComponent.hpp"

namespace mixr {
namespace models {

class IAtmosphere : public base::IComponent
{
   DECLARE_SUBCLASS(IAtmosphere, base::IComponent)

public:
   IAtmosphere();
};

}
}

#endif
