
#ifndef __mixr_models_common_Chaff_HPP__
#define __mixr_models_common_Chaff_HPP__

#include "mixr/models/player/effect/IEffect.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Chaff
// Description: Concrete chaff class
// Factory name: Chaff
//------------------------------------------------------------------------------
class Chaff final: public IEffect
{
    DECLARE_SUBCLASS(Chaff, IEffect)

public:
    Chaff();

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;
};

}
}

#endif
