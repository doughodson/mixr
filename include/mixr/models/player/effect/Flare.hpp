
#ifndef __mixr_models_common_Flare_HPP__
#define __mixr_models_common_Flare_HPP__

#include "mixr/models/player/effect/IEffect.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Flare
// Description: Concrete flare class
// Factory name: Flare
//------------------------------------------------------------------------------
class Flare final: public IEffect
{
    DECLARE_SUBCLASS(Flare, IEffect)

public:
    Flare();

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
};

}
}

#endif
