
#ifndef __mixr_graphics_LongitudeReadout_HPP__
#define __mixr_graphics_LongitudeReadout_HPP__

#include "DirectionReadout.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: LongitudeReadout
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: LongitudeReadout
// Slots: none
//------------------------------------------------------------------------------
// Notes:
//    1) see the 'DirectionReadout' notes
//    2) The '+ char is replaced with 'E', and the '-' char is replaced with 'W'
//
// Example formats:
//
//    0DD@            // Degrees
//    0DD.D@          // Degrees
//
//    0DD@MM          // Degrees and minutes
//    0DD@MM.MM       // Degrees and minutes
//
//    0DD@MM'SS       // Degrees, minutes and seconds
//    0DD@MM'SS.SSS   // Degrees, minutes and seconds
//
//    +0DD@MM'SS.S    // Degrees, minutes and seconds with '+' on positive values
//    0DD@MM'SS.S+    //  ... and with the '+' or '-' character as a suffix
//
//------------------------------------------------------------------------------
class LongitudeReadout final: public DirectionReadout
{
    DECLARE_SUBCLASS(LongitudeReadout, DirectionReadout)
public:
    LongitudeReadout();
    char filterInputEvent(const int event, const char) final;
protected:
   //virtual void makeText();
};

}
}

#endif
