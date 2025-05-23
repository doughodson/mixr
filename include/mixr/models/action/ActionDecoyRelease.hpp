#ifndef __mixr_models_ActionDecoyRelease_HPP__
#define __mixr_models_ActionDecoyRelease_HPP__

#include "mixr/models/action/IAction.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class Integer; class INumber; }
namespace models {
class OnboardComputer;

//------------------------------------------------------------------------------
// Class: ActionDecoyRelease
// Description:  Concrete - releases a decoy
//
// Factory name: ActionDecoyRelease
// Slots:
//   numToLaunch  <Integer>  ! number of decoys to launch (default: 1)
//   interval     <INumber>  ! time, in seconds, between launches (default: 0)
//
//------------------------------------------------------------------------------
class ActionDecoyRelease final: public IAction
{
    DECLARE_SUBCLASS(ActionDecoyRelease, IAction)

public:
    ActionDecoyRelease();

    // set functions
    virtual bool setInterval(const double x)              { interval = x; return true; }
    virtual bool setNumToLaunch(const int x)              { numToLaunch = x; return true; }
    void process(const double dt) override;

    bool trigger(OnboardComputer* const mgr) override;

private:
    int numToLaunch {1};   // how many decoys to launch this action?
    double interval {};    // seconds delay between launch... default is 0
    double tod {};         // the sim time of day (we are going to add to it)
    double startTOD {};    // our starting tod (that way we know when we've elapsed so far)

private:
   bool setSlotInterval(const base::INumber*);
   bool setSlotNumToLaunch(const base::Integer*);
};

}
}

#endif
