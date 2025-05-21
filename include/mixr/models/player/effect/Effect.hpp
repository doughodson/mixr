
#ifndef __mixr_models_common_Effect_HPP__
#define __mixr_models_common_Effect_HPP__

#include "mixr/models/player/weapon/IWeapon.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Effect
// Description: Base class for effects (chaff, flares, decoys, etc).
//
//    Even though they're not weapons, the Effect class is derived from
//    the Weapon class because their basic behaviours are the same.  That is,
//    they can be jettisoned and released, or pre-released() and be become
//    an independent player.  They can also be managed by a Stores class.
//
// Factory name: Effect
// Slots:
//    dragIndex   <INumber>   ! drag index used by default dynamics (default: 0.0006)
//
//------------------------------------------------------------------------------
class Effect : public IWeapon
{
    DECLARE_SUBCLASS(Effect, IWeapon)

public:
    Effect();

    double getDragIndex() const                    { return dragIndex; }
    void setDragIndex(const double v)              { dragIndex = v; }

    const char* getDescription() const override;
    const char* getNickname() const override;
    int getCategory() const override;

    bool collisionNotification(IPlayer* const p) override;
    bool crashNotification() override;

protected:
    void weaponDynamics(const double dt) override;
    void updateTOF(const double dt) override;

private:
    double dragIndex {0.0006};   // Drag Index

private:
   // slot table helper methods
   bool setSlotDragIndex(base::INumber* const);
};

}
}

#endif
