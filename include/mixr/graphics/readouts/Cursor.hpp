
#ifndef __mixr_graphics_Cursor_H__
#define __mixr_graphics_Cursor_H__

#include "mixr/graphics/readouts/Readout.hpp"

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
class Cursor final: public Readout
{
    DECLARE_SUBCLASS(Cursor, Readout)

public:
   Cursor();
   void updateData(const double dt = 0.0) final;
};

}
}

#endif
