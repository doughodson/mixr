
#ifndef __mixr_models_Flare_H__
#define __mixr_models_Flare_H__

#include "mixr/models/player/effect/Effect.hpp"

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
