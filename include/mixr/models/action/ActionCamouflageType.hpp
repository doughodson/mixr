#ifndef __mixr_models_ActionCamouflageType_HPP__
#define __mixr_models_ActionCamouflageType_HPP__

#include "mixr/models/action/Action.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class Integer; }
namespace models {
class OnboardComputer;

//------------------------------------------------------------------------------
// Class: ActionCamouflageType
// Description: Concrete - On trigger, switches our ownship's camouflage type
//
// Factory name: ActionCamouflageType
// Slots:
//   camouflageType  <Integer>  ! User-defined camouflage type (positive integer or zero for none)
//                              ! (default: 0)
//
//------------------------------------------------------------------------------
class ActionCamouflageType final: public Action
{
    DECLARE_SUBCLASS(ActionCamouflageType, Action)

public:
   ActionCamouflageType();

   unsigned int getCamouflageType() const   { return camouflage; }         // Returns the user defined camouflage type (or zero for none)
   virtual bool setCamouflageType(const unsigned int);                     // Sets the user defined camouflage type (or zero for none)

   bool trigger(OnboardComputer* const mgr) override;

private:
   unsigned int camouflage {};    // Camouflage type (0 is none)

private:
   // slot table helper methods
   bool setSlotCamouflageType(const base::Integer* const);   // Sets user defined camouflage type
};

}
}

#endif
