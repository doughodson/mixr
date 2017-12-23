
#ifndef __mixr_models_Aam_H__
#define __mixr_models_Aam_H__

#include "mixr/models/player/weapon/Missile.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Aam
// Description: Base class for Air to Air (A/A)
//
//    Contains a simple aero and guidance model, which derived models can
//    override at will.
//
// Factory name: AamMissile
//------------------------------------------------------------------------------
class Aam : public Missile
{
    DECLARE_SUBCLASS(Aam, Missile)

public:
    Aam();

    const char* getDescription() const override;  // Returns "AIM"
    const char* getNickname() const override;     // Returns "GenericAam"
    int getCategory() const override;             // Returns MISSILE or GUIDED
};

}
}

#endif
