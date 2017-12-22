
#ifndef __mixr_instruments_SolenoidSwitch_H__
#define __mixr_instruments_SolenoidSwitch_H__

#include "mixr/graphics/Graphic.hpp"
#include <array>

namespace mixr {
namespace base { class UpTimer; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: SolenoidSwitch
//
// Description: This is a custom button manager, which will hold 3 buttons, and
// will time when the user holds up or down from the center on the button, and will
// switch at that time.  A click on the center button will simply return to the
// center position (no holding).
// You can retrieve the button position at any time, or
//------------------------------------------------------------------------------
class SolenoidSwitch : public graphics::Graphic
{
    DECLARE_SUBCLASS(SolenoidSwitch, graphics::Graphic)

public:
    SolenoidSwitch();

    virtual bool setHoldTimer(const double);

    // button enums
    enum { CENTER_BUTTON = 1, TOP_BUTTON = 2, BOTTOM_BUTTON = 3, NUM_BUTTONS = 3 };

    // set functions for setting latched
    void setOkToLatch(bool x)   { latched = x; }
    bool okToLatch() const      { return latched; }

    // hit when our buttons are pressed and we are trying to "latch" the switch in place
    // (either by timer or logic)
    void latch(const int buttonId);
    // button is release, switch will determine if latch will "stick"
    void determineLatch();
    // return to initial spot
    void resetButton();
    // get function
    int getSwitchPosition()   { return currButtonId; }
    base::UpTimer* getTimer() { return timer; }

    void updateData(const double dt = 0.0) override;
    bool event(const int event, base::Object* const obj = nullptr) override;

protected:
    // event function
    bool selectLatch(const base::Number* const x);

private:
    std::array<bool, NUM_BUTTONS> picked {};     // tells our buttons if they are currently picked or not
    std::array<SendData, NUM_BUTTONS> pickedSD;
    std::array<int, NUM_BUTTONS> eventMap {};    // actual event id we want to generate for each button hit
    int currButtonId {CENTER_BUTTON};            // button ID of our current hold button (one being clicked)
    int lastButtonId {CENTER_BUTTON};            // button ID of the last button that was "pushed"
    base::UpTimer* timer {};                     // our selection timer
    bool latched {};                             // our latch flag, which, without a timer, will be a logical
                                                 // flag to determine when to keep the switch up or down, or
                                                 // make the switch return to it's starting position

private:
    // slot table helper methods
    bool setSlotHoldTimer(const base::Number* const);
    bool setSlotEventMap(const base::PairStream* const);
};

}
}

#endif
