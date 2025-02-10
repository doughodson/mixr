
#ifndef __mixr_base_ubf_Arbiter_HPP__
#define __mixr_base_ubf_Arbiter_HPP__

#include "IBehavior.hpp"

namespace mixr {
namespace base {
class List;

namespace ubf {
class IState;
class IAction;

//------------------------------------------------------------------------------
// Class: Arbiter
// Description:
//    A meta-behavior that generates a "complex action" based on the actions
//    generated our list of behaviors.
//------------------------------------------------------------------------------
// Factory name: Arbiter
//------------------------------------------------------------------------------
// Slots:
//    behaviors   <PairStream>      ! List of behaviors
//------------------------------------------------------------------------------
// Notes:
//    The default is to select the Action with the highest vote value.
//------------------------------------------------------------------------------
class Arbiter : public IBehavior
{
   DECLARE_SUBCLASS(Arbiter, IBehavior)

public:
   Arbiter();

   IAction* genAction(const IState* const state, const double dt) override;

protected:
   base::List* getBehaviors();

   // evaluates a list of actions and return an optional "complex action"
   // (default: returns the action with the highest vote value)
   virtual IAction* genComplexAction(List* const actionSet);

   // add new behavior to list
   void addBehavior(IBehavior* const);

private:
   base::List* behaviors {};

private:
   // slot table helper methods
   bool setSlotBehaviors(base::PairStream* const);
};

inline base::List* Arbiter::getBehaviors()                 { return behaviors; }

}
}
}

#endif

