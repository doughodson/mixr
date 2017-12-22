
#ifndef __mixr_instruments_RotarySwitch_H__
#define __mixr_instruments_RotarySwitch_H__

#include "mixr/instruments/buttons/Button.hpp"
#include <array>

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class:       RotarySwitch
// Description: Generic multi-position selector switch that knows when it is left or right
//                mouse clicked and moves to next or previous switch position.
//------------------------------------------------------------------------------
class RotarySwitch : public Button
{
   DECLARE_SUBCLASS(RotarySwitch,Button)

public:
   RotarySwitch();

   // Get current switch position
   int getCurrentPosition() const         { return currentPosition; };

   bool onSingleClick() override;

   void updateData(const double dt = 0.0) override;

private:
   static const int MAX_ANGLES = 500;

   int currentPosition {1};    //current switch position
   std::array<double, MAX_ANGLES> angles {};
   int numAngs {};
   int startPosition {1};
   SendData angleSD;       // angle to send to our rotator

private:
   bool setSlotAngles(const base::PairStream* const);
   bool setSlotStartPosition(const base::Number* const);
};

}
}

#endif
