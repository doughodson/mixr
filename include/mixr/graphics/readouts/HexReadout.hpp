
#ifndef __mixr_graphics_HexReadout_HPP__
#define __mixr_graphics_HexReadout_HPP__

#include "NumericReadout.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: HexReadout
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: HexReadout
// Slots: none
//------------------------------------------------------------------------------
// Example formats:
//
//    ######    // Hex number
//    00000#    // Hex number w/leading zeros
//
//------------------------------------------------------------------------------
class HexReadout final: public NumericReadout
{
   DECLARE_SUBCLASS(HexReadout, NumericReadout)

public:
   HexReadout();

   void makeText() final;
   char filterInputEvent(const int event, const char) final;
   double getInputValue() const final;

protected:
   void reformat(const char* const example) final;
};

}
}

#endif
