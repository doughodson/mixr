
#ifndef __mixr_graphics_Rotary_H__
#define __mixr_graphics_Rotary_H__

#include "mixr/graphics/readouts/AbstractField.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Rotary
//
// Description: Rotary text
//
// Factory name: Rotary
//
//------------------------------------------------------------------------------
class Rotary : public AbstractField
{
    DECLARE_SUBCLASS(Rotary, AbstractField)

public:
   Rotary();
   void draw() override;

private:
    // this flag tells us our components need to be pre-drawn (to avoid flicker)
    bool preDrawSelectList {true};
};

}
}

#endif
