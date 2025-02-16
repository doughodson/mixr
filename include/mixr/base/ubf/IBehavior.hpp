
#ifndef __mixr_base_ubf_IBehavior_HPP__
#define __mixr_base_ubf_IBehavior_HPP__

#include "mixr/base/IComponent.hpp"

namespace mixr {
namespace base {
class Integer;
namespace ubf {
class IState;
class IAction;

//------------------------------------------------------------------------------
// Class: IBehavior
// Description: Interface class for all behaviors.  Generates an optional
//              action based on our current state.
//------------------------------------------------------------------------------
// Factory name: IBehavior
//------------------------------------------------------------------------------
// Slots:
//    vote     <Integer>   ! default vote/weight value for actions generated
//                         ! by this behavior
//------------------------------------------------------------------------------
class IBehavior : public base::IComponent
{
   DECLARE_SUBCLASS(IBehavior, base::IComponent)

public:
   IBehavior();

   // Returns a pre-ref'd Action (or zero if no action is generated)
   virtual IAction* genAction(const IState* const state, const double dt) = 0;

protected:
   int getVote() const;
   virtual void setVote(const int x);

private:
   int vote{};

private:
   // slot table helper methods
   bool setSlotVote(const base::Integer* const);
};

inline void IBehavior::setVote(const int x)    { vote = x; }
inline int IBehavior::getVote() const          { return vote; }

}
}
}

#endif
