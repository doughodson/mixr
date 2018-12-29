
#ifndef __mixr_graphics_OctalReadout_H__
#define __mixr_graphics_OctalReadout_H__

#include "NumericReadout.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: OctalReadout
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: OctalReadout
//------------------------------------------------------------------------------
// Example formats:
//
//    ######    // Octal number
//    00000#    // Octal number w/leading zeros
//
//------------------------------------------------------------------------------
class OctalReadout final: public NumericReadout
{
   DECLARE_SUBCLASS(OctalReadout, NumericReadout)

public:
   OctalReadout();
   void makeText() final;
   char filterInputEvent(const int event, const char tc) final;
   double getInputValue() const final;

protected:
   void reformat(const char* const example) final;
};

}
}

#endif
