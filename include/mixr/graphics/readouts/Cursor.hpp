
#ifndef __mixr_graphics_Cursor_H__
#define __mixr_graphics_Cursor_H__

#include "mixr/graphics/readouts/AbstractField.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Cursor
//
// Factory name: Cursor
//------------------------------------------------------------------------------
class Cursor : public AbstractField
{
    DECLARE_SUBCLASS(Cursor, AbstractField)

public:
   Cursor();
   void updateData(const double dt = 0.0) override;
};

}
}

#endif
