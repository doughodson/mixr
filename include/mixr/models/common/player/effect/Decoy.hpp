
#ifndef __mixr_models_common_Decoy_HPP__
#define __mixr_models_common_Decoy_HPP__

#include "mixr/models/common/player/effect/Effect.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Decoy
// Description: Generic decoy class
// Factory name: Decoy
//------------------------------------------------------------------------------
class Decoy : public Effect
{
    DECLARE_SUBCLASS(Decoy, Effect)

public:
    Decoy();

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
};

}
}

#endif
