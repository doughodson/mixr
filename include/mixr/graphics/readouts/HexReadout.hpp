
#ifndef __mixr_graphics_HexReadout_H__
#define __mixr_graphics_HexReadout_H__

#include "NumericReadout.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: HexReadout
//
// Factory name: HexReadout
//
// Example formats:
//
//    ######    // Hex number
//    00000#    // Hex number w/leading zeros
//
//------------------------------------------------------------------------------
class HexReadout : public NumericReadout
{
   DECLARE_SUBCLASS(HexReadout, NumericReadout)

public:
   HexReadout();

   void makeText() override;
   char filterInputEvent(const int event, const int tc) override;
   double getInputValue() const override;

protected:
   void reformat(const char* const example) override;
};

}
}

#endif
