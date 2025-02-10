
#ifndef __mixr_base_ubf_IState_HPP__
#define __mixr_base_ubf_IState_HPP__

#include "mixr/base/Component.hpp"

namespace mixr {
namespace base {
namespace ubf {

//------------------------------------------------------------------------------
// Class: IState
// Description: The actor's state vector, as seen by the Behaviors.
//------------------------------------------------------------------------------
// Factory name: IState
//------------------------------------------------------------------------------
class IState : public base::Component
{
   DECLARE_SUBCLASS(IState, base::Component)
public:
   IState();

   virtual void updateGlobalState();
   virtual void updateState(const base::Component* const actor);

   virtual const IState* getUbfStateByType(const std::type_info& type) const;
};

}
}
}

#endif
