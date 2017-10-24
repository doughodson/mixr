
#ifndef __mixr_models_Chaff_H__
#define __mixr_models_Chaff_H__

#include "mixr/models/player/effect/Effect.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Chaff
// Description: Generic chaff class
// Factory name: Chaff
//------------------------------------------------------------------------------
class Chaff : public Effect
{
    DECLARE_SUBCLASS(Chaff, Effect)

public:
    Chaff();

    virtual const char* getDescription() const override;
    virtual const char* getNickname() const override;
    virtual int getCategory() const override;
};

}
}

#endif
