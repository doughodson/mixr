
#ifndef __mixr_models_common_Flare_HPP__
#define __mixr_models_common_Flare_HPP__

#include "mixr/models/common/player/effect/Effect.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Flare
// Description: Generic flare class
// Factory name: Flare
//------------------------------------------------------------------------------
class Flare : public Effect
{
    DECLARE_SUBCLASS(Flare, Effect)

public:
    Flare();

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
};

}
}

#endif
