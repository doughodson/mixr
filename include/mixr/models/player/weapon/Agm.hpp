
#ifndef __mixr_models_Agm_H__
#define __mixr_models_Agm_H__

#include "mixr/models/player/weapon/Missile.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Agm
// Description: Base class for Air to Ground (A/G) missiles
//
//    Contains a simple aero and guidance model, which derived models can
//    override at will.
//
// Factory name: AgmMissile
//------------------------------------------------------------------------------
class Agm : public Missile
{
    DECLARE_SUBCLASS(Agm, Missile)

public:
    Agm();

    const char* getDescription() const override;  // returns "AGM"
    const char* getNickname() const override;     // returns "GenericAgm"
    int getCategory() const override;             // returns MISSILE | GUIDED

private:
   virtual bool calculateVectors(const Player* const tgt, const Track* const trk, base::Vec3d* const los,
          base::Vec3d* const vel, base::Vec3d* const posx) const override;
};

}
}

#endif
