
#ifndef __mixr_graphics_Rotary_HPP__
#define __mixr_graphics_Rotary_HPP__

#include "mixr/graphics/readouts/IReadout.hpp"

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
class Rotary final: public IReadout
{
    DECLARE_SUBCLASS(Rotary, IReadout)

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
