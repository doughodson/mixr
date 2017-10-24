
#ifndef __mixr_models_Decoy_H__
#define __mixr_models_Decoy_H__

#include "mixr/models/player/effect/Effect.hpp"

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

    virtual const char* getDescription() const override;
    virtual const char* getNickname() const override;
    virtual int getCategory() const override;
};

}
}

#endif
