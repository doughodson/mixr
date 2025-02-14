#include "mixr/instruments/buttons/Button.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/graphics/Display.hpp"

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(Button, "Button")
EMPTY_DELETEDATA(Button)

BEGIN_SLOTTABLE(Button)
    "eventId",        // Which event we will send for each button (A, B, C events)
END_SLOTTABLE(Button)

BEGIN_SLOT_MAP(Button)
    ON_SLOT(1, setSlotEventId, base::INumber)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Button)
    ON_EVENT(ON_SINGLE_CLICK, onSingleClick)
    ON_EVENT(ON_CANCEL, onCancel)
END_EVENT_HANDLER()

Button::Button()
{
    STANDARD_CONSTRUCTOR()
}

void Button::copyData(const Button& org, const bool)
{
    BaseClass::copyData(org);
    eventId = org.eventId;
}

//------------------------------------------------------------------------------
// setSlotEventId() - sets our slot event Id
//------------------------------------------------------------------------------
bool Button::setSlotEventId(const base::INumber* const newEvent)
{
    bool ok = false;
    if (newEvent != nullptr) {
        int a = newEvent->asInt();
        ok = setEventId(a);
    }
    return ok;
}

//------------------------------------------------------------------------------
// onSingleClick() - tells us we have been clicked, and we can override this
// to make it do whatever we want.
//------------------------------------------------------------------------------
bool Button::onSingleClick()
{
    // when I am clicked, I will send an event to my container, we find out what
    // event Id we have, and send that eventId
    bool ok = false;
    graphics::Display* myDisplay = (graphics::Display*)findContainerByType(typeid(graphics::Display));

    if (myDisplay != nullptr) {
        myDisplay->buttonEvent(getEventId());
        ok = true;
    }

    return ok;
}

//------------------------------------------------------------------------------
// onCancel() - this is where we cancel button pushes
//------------------------------------------------------------------------------
bool Button::onCancel()
{
    // do nothing here, but our pushbuttons and switches will!
    return true;
}

}
}
