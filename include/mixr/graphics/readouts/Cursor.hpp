
#ifndef __mixr_graphics_Cursor_H__
#define __mixr_graphics_Cursor_H__

#include "mixr/graphics/readouts/AbstractReadout.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Cursor
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Cursor
// Slots: none
//------------------------------------------------------------------------------
class Cursor final: public AbstractReadout
{
    DECLARE_SUBCLASS(Cursor, AbstractReadout)

public:
   Cursor();
   void updateData(const double dt = 0.0) final;
};

}
}

#endif
