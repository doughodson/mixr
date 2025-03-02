
#ifndef __mixr_instruments_SolenoidButton_HPP__
#define __mixr_instruments_SolenoidButton_HPP__

#include "mixr/instruments/buttons/Button.hpp"

namespace mixr {
namespace base { class Boolean; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: SolenoidButton
// Description: Custom button that understands that it belongs to a SolenoidSwitch,
// and will let it know when its respective button has been hit.
//------------------------------------------------------------------------------
class SolenoidButton : public Button
{
    DECLARE_SUBCLASS(SolenoidButton, Button)
public:
    SolenoidButton();

    bool onSingleClick() override;

    // The left mouse button has been depressed
    virtual bool onMouseDown();
    virtual bool onPicked(const base::Boolean* const);

    bool event(const int event, base::IObject* const obj = nullptr) override;
    void updateData(const double dt = 0.0) override;

private:
    bool noTimer{};    // this flag tells this button that we don't time, we simply switch
    bool pushed{};     // flag that tells us we have the mouse down on our current graphic
    SendData pushedSD;
};

}
}

#endif
