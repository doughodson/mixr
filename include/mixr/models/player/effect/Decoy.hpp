
#ifndef __mixr_models_common_Decoy_HPP__
#define __mixr_models_common_Decoy_HPP__

#include "mixr/models/player/effect/IEffect.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Decoy
// Description: Concrete decoy class
// Factory name: Decoy
//------------------------------------------------------------------------------
class Decoy final: public IEffect
{
    DECLARE_SUBCLASS(Decoy, IEffect)

public:
    Decoy();

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
};

}
}

#endif
