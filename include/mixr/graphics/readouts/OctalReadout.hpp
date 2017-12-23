
#ifndef __mixr_graphics_OctalReadout_H__
#define __mixr_graphics_OctalReadout_H__

#include "NumericReadout.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: OctalReadout
//
// Factory name: OctalReadout
//
// Example formats:
//
//    ######    // Octal number
//    00000#    // Octal number w/leading zeros
//
//------------------------------------------------------------------------------
class OctalReadout : public NumericReadout
{
   DECLARE_SUBCLASS(OctalReadout, NumericReadout)

public:
   OctalReadout();
   void makeText() override;
   char filterInputEvent(const int event, const int tc) override;
   double getInputValue() const override;

protected:
   void reformat(const char* const example) override;
};

}
}

#endif
