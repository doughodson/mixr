
#ifndef __mixr_instruments_Switch_H__
#define __mixr_instruments_Switch_H__

#include "mixr/instruments/buttons/Button.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: Switch
// Description: Button that can have multiple formats, up to as many as the user
// wants.
//------------------------------------------------------------------------------
class Switch : public Button
{
    DECLARE_SUBCLASS(Switch,Button)

public:
    Switch();

    // get functions
    int getCurrentState() const   { return currentState; }

    // set functions
    virtual bool setNumSelections(const int);
    virtual bool setCurrentState(const int);

    bool onSingleClick() override;

    void updateData(const double dt = 0.0) override;

private:
    int currentState{1};     // which state are we currently in?
    int numSelections{1};    // number of selections to go through
    SendData currentStateSD;

private:
    bool setSlotNumSelections(const base::Number* const);
    bool setSlotCurrentState(const base::Number* const);
};

}
}

#endif
