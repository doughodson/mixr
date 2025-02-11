
#ifndef __mixr_graphics_Cursor_HPP__
#define __mixr_graphics_Cursor_HPP__

#include "mixr/graphics/readouts/IReadout.hpp"

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
class Cursor final: public IReadout
{
    DECLARE_SUBCLASS(Cursor, IReadout)

public:
   Cursor();
   void updateData(const double dt = 0.0) final;
};

}
}

#endif
