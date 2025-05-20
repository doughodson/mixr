
#ifndef __mixr_models_Action_HPP__
#define __mixr_models_Action_HPP__

#include "mixr/base/ubf/IAction.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class IComponent; }
namespace models {
class OnboardComputer;

//------------------------------------------------------------------------------
// Class: Action
// Description: Actions that occur at steerpoints.
//
// Notes:
//
//  1) Use process() to process one action frame of delta time, 'dt'.  Follow
//  standard real-time rules since process() may be called from updateTC().
//
//  2) Actions that are not ready to start (isReadyToStart()) may be skipped
//     by the manager.
//
//  3) One shot action will complete during the call to trigger().
//
//  4) Any action that will take time to complete must have a manager
//     passed via trigger().
//
//  5) Derived from an UBF action (see "mixr/base/ubf/IAction.hpp").
//  The execute() function's 'actor' must be our OnboardComputer or our
//  ownship, which can be  used to find our OnboardComputer.  The execute()
//  function will find the OnboardComputer and 'trigger()' the action.
//
//------------------------------------------------------------------------------
class Action : public base::ubf::IAction
{
    DECLARE_SUBCLASS(Action, base::ubf::IAction)

public:
   Action();

   virtual bool isReadyToStart();                               // True if action is ready to start
   virtual bool isInProgress();                                 // True if action is in progress
   virtual bool isCompleted();                                  // True if action has been completed

   virtual bool trigger(OnboardComputer* const);                // Starts this action
   virtual bool cancel();                                       // Cancels this action
   virtual void process(const double dt);                       // Action processing

   int getRefId() const                  { return refId; }      // Message Ref ID
   virtual void setRefId(const int);                            // Sets the message ref ID

   bool execute(base::IComponent* actor) override;

protected:
   OnboardComputer* getManager()   { return manager; }          // Our manager
   bool setManager(OnboardComputer* const);                     // Set our manager

   virtual void setCompleted(const bool);                       // Sets the completed flag

private:
   base::safe_ptr<OnboardComputer> manager;   // Our manager (only set while we're in progress)
   int refId {};                              // Ref ID
   bool completed {};                         // True if action has been completed
};

}
}

#endif
