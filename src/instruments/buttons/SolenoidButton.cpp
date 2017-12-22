
#include "mixr/instruments/buttons/SolenoidButton.hpp"

#include "mixr/instruments/buttons/SolenoidSwitch.hpp"

#include "mixr/graphics/Display.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Timers.hpp"

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(SolenoidButton, "SolenoidButton")
EMPTY_SLOTTABLE(SolenoidButton)
EMPTY_DELETEDATA(SolenoidButton)

BEGIN_EVENT_HANDLER(SolenoidButton)
   ON_EVENT(INPUT_LEFT_EDGE, onMouseDown)
   ON_EVENT_OBJ(USER_KEY_EVENT, onPicked, base::Number)
END_EVENT_HANDLER()

SolenoidButton::SolenoidButton()
{
   STANDARD_CONSTRUCTOR()
   pushedSD.empty();
}

void SolenoidButton::copyData(const SolenoidButton& org, const bool)
{
    noTimer = org.noTimer;
    pushed = org.pushed;
    pushedSD.empty();
}

//------------------------------------------------------------------------------
// onMouseDown() - when the mouse is pressed
//------------------------------------------------------------------------------
bool SolenoidButton::onMouseDown()
{
    if (!noTimer) {
        // tell our switch to latch
        SolenoidSwitch* hs = (SolenoidSwitch*)findContainerByType(typeid(SolenoidSwitch));
        if (hs != nullptr) hs->latch(getEventId());
    }
    return true;
}

//------------------------------------------------------------------------------
// onSingleClick() - override this to talk to our Hold Switch, not our display
//------------------------------------------------------------------------------
bool SolenoidButton::onSingleClick()
{
    if (!noTimer) {
        // if we are timing, we need to tell our switch that it needs to start timing
        SolenoidSwitch* hs = (SolenoidSwitch*)findContainerByType(typeid(SolenoidSwitch));
        if (hs != nullptr) hs->determineLatch();
    }
    return true;
}

bool SolenoidButton::onPicked(const base::Number* const x)
{
    if (x != nullptr) pushed = x->getBoolean();
    return true;
}

void SolenoidButton::updateData(double dt)
{
    BaseClass::updateData(dt);
    send("push", SELECT, pushed, pushedSD);
}

}
}
