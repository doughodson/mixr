
#ifndef __mixr_graphics_TimeReadout_H__
#define __mixr_graphics_TimeReadout_H__

#include "NumericReadout.hpp"

#include "mixr/graphics/readouts/readout_utils.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: TimeReadout
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: TimeReadout
//------------------------------------------------------------------------------
// Example formats:
//
//    HH:MM:SS      // Hours, minutes and seconds
//    0H:MM:SS.S    // Hours (w/leading zero), minutes and seconds decimal
//   +HH:MM         // Hours (w/sign) and minutes
//    HHMM.M        // Hours (w/sign) and minutes decimal
//   +0HMM:SS.S     // Hours (w/signed and leading zero), minutes
//                  //   and seconds decimal
//    MMSS.S        // Minutes and seconds decimal
//    SSSS          // Seconds
//
// Notes: The seconds and third numbers always have leading zeros.  The first
// number requires a zero, '0', to have leading zeros.
//
//------------------------------------------------------------------------------
class TimeReadout final: public NumericReadout
{
   DECLARE_SUBCLASS(TimeReadout, NumericReadout)

public:
   TimeReadout();
   char filterInputEvent(const int event, const char) final;
   double getInputValue() const final;

protected:
   void makeText() final;
   void reformat(const char* const example) final;

private:
   TimeMode tmode {TimeMode::hhmmss};
};

}
}

#endif
