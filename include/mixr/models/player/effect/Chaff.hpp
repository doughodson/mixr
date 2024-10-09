
#ifndef __mixr_models_common_Chaff_HPP__
#define __mixr_models_common_Chaff_HPP__

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

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
};

}
}

#endif
