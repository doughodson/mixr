
#ifndef __mixr_graphics_Rotary_H__
#define __mixr_graphics_Rotary_H__

#include "mixr/graphics/readouts/Readout.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Rotary
// Description: Rotary text
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Rotary
// Slots: none
//------------------------------------------------------------------------------
class Rotary final: public Readout
{
    DECLARE_SUBCLASS(Rotary, Readout)

public:
   Rotary();
   void draw() final;

private:
    // this flag tells us our components need to be pre-drawn (to avoid flicker)
    bool preDrawSelectList{true};
};

}
}

#endif
