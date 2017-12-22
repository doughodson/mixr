
#ifndef __mixr_instruments_PushButton_H__
#define __mixr_instruments_PushButton_H__

#include "mixr/instruments/buttons/Button.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: PushButton
// Description: Button which will select one or another graphic based on being pressed or not.
//------------------------------------------------------------------------------
class PushButton : public Button
{
    DECLARE_SUBCLASS(PushButton,Button)

public:
    PushButton();

   // gets the pushbutton type ie momentary = false or maintained = true
   bool getFunction() const     { return functionType; };

   // gets the current state of the pushbutton ie off = false or on = true
   bool getCurrentState() const { return currentState; };

   // sets the pushbutton type ie momentary = false or maintained = true
   virtual bool setFunction(const bool);

   // The left mouse button has been depressed
   virtual bool onMouseDown();

   bool onSingleClick() override;
   bool onCancel() override;

   void updateData(const double dt = 0.0) override;
   bool event(const int event, base::Object* const obj = nullptr) override;

private:
   bool functionType {};     // false = momentary, true = maintained
   bool currentState {};     // false = off, true = on
   bool mouseDown {};        // mouse press down status
   SendData buttonStatusSD;  // what is our button doing?
   bool initState {};        // initial state

private:
   // slot table helper methods
   // sets the pushbutton type to momentary = false or maintained = true
   bool setSlotFunction(const base::Number* const);
   bool setSlotStartState(const base::Number* const);
};

}
}

#endif
