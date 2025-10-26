
#include "mixr/base/ubf/Arbiter.hpp"
#include "mixr/base/ubf/IAction.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/IPairStream.hpp"

namespace mixr {
namespace base {
namespace ubf {

IMPLEMENT_SUBCLASS(Arbiter, "Arbiter")
EMPTY_COPYDATA(Arbiter)

BEGIN_SLOTTABLE(Arbiter)
   "behaviors"                    //  1) behaviors
END_SLOTTABLE(Arbiter)

BEGIN_SLOT_MAP(Arbiter)
   ON_SLOT(1, setSlotBehaviors, base::IPairStream)
END_SLOT_MAP()

Arbiter::Arbiter()
{
   STANDARD_CONSTRUCTOR()
   behaviors = new base::List();
}

void Arbiter::deleteData()
{
   // unref behaviors
   if ( behaviors!=nullptr ) { behaviors->unref();   behaviors = nullptr; }
}

//------------------------------------------------------------------------------
// genAction() - generate an action
//------------------------------------------------------------------------------
IAction* Arbiter::genAction(const IState* const state, const double dt)
{
   // create list for action set
   const auto actionSet{new base::List()};

   // fill out list of recommended actions by behaviors
   base::IList::Item* item{behaviors->getFirstItem()};
   while (item != nullptr) {
      // get a behavior
      const auto behavior{static_cast<IBehavior*>(item->getValue())};
      // generate action, we have reference
      IAction* action{behavior->genAction(state, dt)};
      if (action != nullptr) {
         // add to action set
         actionSet->addTail(action);
         // unref our action reference
         action->unref();
      }
      // goto behavior
      item = item->getNext();
   }

   // given the set of recommended actions, the arbiter
   // decides what action to take
   IAction* complexAction{genComplexAction(actionSet)};

   // done with action set
   actionSet->unref();

   // return action to perform
   return complexAction;
}


//------------------------------------------------------------------------------
// Default: select the action with the highest vote
//------------------------------------------------------------------------------
IAction* Arbiter::genComplexAction(base::IList* const actionSet)
{
   IAction* complexAction{};
   int maxVote{};

   // process entire action set
   base::IList::Item* item{actionSet->getFirstItem()};
   while (item != nullptr) {

      // Is this action's vote higher than the previous?
      const auto action = static_cast<IAction*>(item->getValue());
      if (maxVote==0 || action->getVote() > maxVote) {

         // Yes ...
         if (complexAction != nullptr) complexAction->unref();
         complexAction = action;
         complexAction->ref();
         maxVote = action->getVote();
      }

      // next action
      item = item->getNext();
   }

   if (maxVote > 0 && isMessageEnabled(MSG_DEBUG))
      std::cout << "Arbiter: chose action with vote= " << maxVote << std::endl;

   // Use our vote value; if its been set
   if (getVote() > 0 && complexAction != nullptr) {
      complexAction->setVote(getVote());
   }

   // complexAction will have the vote value of whichever component action was selected
   return complexAction;
}

//------------------------------------------------------------------------------
// addBehavior() - add a new behavior
//------------------------------------------------------------------------------
void Arbiter::addBehavior(IBehavior* const x)
{
   behaviors->addTail(x);
   x->container(this);
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool Arbiter::setSlotBehaviors(base::IPairStream* const x)
{
   bool ok{true};

   // First, make sure they are all behaviors
   {
      base::IList::Item* item{x->getFirstItem()};
      while (item != nullptr && ok) {
         const auto pair{static_cast<base::Pair*>(item->getValue())};
         item = item->getNext();
         const auto b{dynamic_cast<IBehavior*>(pair->object())};
         if (b == nullptr) {
            // Item is NOT a behavior
            std::cerr << "setSlotBehaviors: slot: " << (*pair).slot() << " is NOT of a Behavior type!" << std::endl;
            ok = false;
         }
      }
   }

   // next, add behaviors to our list
   if (ok) {
      base::IList::Item* item{x->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         item = item->getNext();
         const auto b = static_cast<IBehavior*>(pair->object());
         addBehavior(b);
      }
   }

   return ok;
}

}
}
}
