
#include "mixr/instruments/buttons/SolenoidSwitch.hpp"

#include "mixr/graphics/Display.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Timers.hpp"

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(SolenoidSwitch, "SolenoidSwitch")

BEGIN_SLOTTABLE(SolenoidSwitch)
    "holdTimeSec",      // how long to hold the button before the timer goes off
    "eventMap",         // turns our button "sticks" into events, from position 1 to 3 (center, top, bottom)
END_SLOTTABLE(SolenoidSwitch)

BEGIN_SLOT_MAP(SolenoidSwitch)
    ON_SLOT(1, setSlotHoldTimer, base::INumber)
    ON_SLOT(2, setSlotEventMap,  base::PairStream)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(SolenoidSwitch)
    ON_EVENT_OBJ(SELECT, selectLatch, base::Boolean)
END_EVENT_HANDLER()

SolenoidSwitch::SolenoidSwitch()
{
    STANDARD_CONSTRUCTOR()

    picked[0] = true;              // center button picked
    for (int i = 0; i < NUM_BUTTONS; i++) {
        pickedSD[i].empty();
        eventMap[i] = -1;
    }
}

void SolenoidSwitch::copyData(const SolenoidSwitch& org, const bool)
{
   BaseClass::copyData(org);

   if (org.timer != nullptr) {
       if (timer != nullptr) {
           timer->unref();
           timer = org.timer->clone();
       }
   }
    for (int i = 0; i < NUM_BUTTONS; i++) {
        pickedSD[i].empty();
        picked[i] = org.picked[i];
        eventMap[i] = org.eventMap[i];
    }
    currButtonId = org.currButtonId;
    lastButtonId = org.lastButtonId;
    latched = org.latched;
}

void SolenoidSwitch::deleteData()
{
    if (timer != nullptr) {
        timer->unref();
        timer = nullptr;
    }
}

//------------------------------------------------------------------------------
// setSlotHoldTimer() - set how long to hold our button before state change
//------------------------------------------------------------------------------
bool SolenoidSwitch::setSlotHoldTimer(const base::INumber* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setHoldTimer(x->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotEventMap() - this sets the list of the eventId's we want generated
// when a button is activated.  For example, if you hold the top button until
// the timer alarms out, instead of sending an event of 2 (top button id), it
// will map that event to this one and will send that to the display to be
// processed.
//------------------------------------------------------------------------------
bool SolenoidSwitch::setSlotEventMap(const base::PairStream* const x)
{
    if (x != nullptr) {
        if (x->entries() != 3) std::cout << "SolenoidSwitch::setSlotEventMap() - Need 3 eventIds for the button, will not send eventIds for the ones without it" << std::endl;
        int count = 0;
        const base::List::Item* item = x->getFirstItem();
        while (item != nullptr && count < 3) {
            const auto pair = static_cast<const base::Pair*>(item->getValue());
            const auto num = dynamic_cast<const base::Integer*>(pair->object());
            if (num != nullptr) eventMap[count] = num->asInt();
            count++;
            item = item->getNext();
        }
        //for (int i = 0; i < NUM_BUTTONS; i++) std::cout << "EVENT ID #" << i << " = " << eventMap[i] << std::endl;
    }
    return true;
}

//------------------------------------------------------------------------------
// selectLatch() - tells our switch if it's ok to "latch"
//------------------------------------------------------------------------------
bool SolenoidSwitch::selectLatch(const base::Boolean* const x)
{
    if (x != nullptr) {
        latched = x->asBool();
    }
    return true;
}


//------------------------------------------------------------------------------
// setHoldTimer() - sets our hold timer
//------------------------------------------------------------------------------
bool SolenoidSwitch::setHoldTimer(const double x)
{
    if (timer != nullptr) timer->alarm(x);
    else {
        timer = new base::UpTimer();
        timer->alarm(x);
    }
    return true;
}

//------------------------------------------------------------------------------
// latch() - called when our button has been pressed and we either want timing to
// begin or awaiting logical latching.
//------------------------------------------------------------------------------
void SolenoidSwitch::latch(const int buttonId)
{
    // clear out our buttons
    if (buttonId == CENTER_BUTTON) resetButton();
    else {
        if (buttonId != currButtonId) {
            for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
            lastButtonId = currButtonId;
            currButtonId = buttonId;
            picked[currButtonId-1] = true;
            // send the event ID IF we are using LOGIC instead of TIMING
            if (timer == nullptr) {
                if (eventMap[currButtonId-1] != -1) {
                    graphics::Display* myDisplay = (graphics::Display*)findContainerByType(typeid(graphics::Display));
                    if (myDisplay != nullptr) {
                        myDisplay->buttonEvent(eventMap[currButtonId-1]);
                        //std::cout << "EVENT ID " << eventMap[currButtonId-1] << " sent!" << std::endl;
                    }
                }
            }
            // if we are using a timer, restart
            else timer->restart();
        }
    }
}

//------------------------------------------------------------------------------
// determineLatch() - called when a button has been released
//------------------------------------------------------------------------------
void SolenoidSwitch::determineLatch()
{
    if (timer != nullptr) {
        if (timer->isRunning()) {
            // ok, we didn't hold the timer long enough, we don't switch
            // we stopped it, so we go back to our last current button
            timer->stop();
            for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
            currButtonId = lastButtonId;
            picked[currButtonId-1] = true;
        }
    }
    // no timer, we are going by logical latch button
    else {
        if (!latched) {
            currButtonId = CENTER_BUTTON;
            resetButton();
        }
    }
}

//------------------------------------------------------------------------------
// resetButton() - this forces a timer stop, and returns to the center
// button position
//------------------------------------------------------------------------------
void SolenoidSwitch::resetButton()
{
    // timer logic
    if (timer != nullptr) {
        if (currButtonId != CENTER_BUTTON) {
            graphics::Display* myDisplay = (graphics::Display*)findContainerByType(typeid(graphics::Display));
            if (myDisplay != nullptr) {
                //std::cout << "EVENT ID " << eventMap[CENTER_BUTTON-1] << " sent!" << std::endl;
                myDisplay->buttonEvent(eventMap[CENTER_BUTTON-1]);
            }
        }
        currButtonId = CENTER_BUTTON;
        lastButtonId = currButtonId;
        timer->stop();
        for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
        picked[currButtonId-1] = true;
    }
    else {
        if (currButtonId != CENTER_BUTTON) {
            graphics::Display* myDisplay = (graphics::Display*)findContainerByType(typeid(graphics::Display));
            if (myDisplay != nullptr) {
                //std::cout << "EVENT ID " << eventMap[CENTER_BUTTON-1] << " sent!" << std::endl;
                myDisplay->buttonEvent(eventMap[CENTER_BUTTON-1]);
            }
        }
        // we are using logic, don't send a reset, just go back to the center
        currButtonId = CENTER_BUTTON;
        lastButtonId = currButtonId;
        for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
        picked[currButtonId-1] = true;
    }
}

//------------------------------------------------------------------------------
// updateData() - background thread
//------------------------------------------------------------------------------
void SolenoidSwitch::updateData(const double dt)
{
    BaseClass::updateData(dt);

    // timer latching
    if (timer != nullptr && timer->alarm()) {
        for (int i = 0; i < NUM_BUTTONS; i++) picked[i] = false;
        lastButtonId = currButtonId;
        picked[currButtonId-1] = true;
        timer->stop();
        if (eventMap[currButtonId-1] != -1) {
            graphics::Display* myDisplay = (graphics::Display*)findContainerByType(typeid(graphics::Display));
            if (myDisplay != nullptr) {
                myDisplay->buttonEvent(eventMap[currButtonId-1]);
                //std::cout << "TIMER EVENT ID " << eventMap[currButtonId-1] << " sent!" << std::endl;
            }
        }
        timer->stop();
        timer->reset();
    }

    // tell our buttons what position they have
    send("button%d", USER_KEY_EVENT, picked.data(), pickedSD.data(), NUM_BUTTONS);
}

}
}
