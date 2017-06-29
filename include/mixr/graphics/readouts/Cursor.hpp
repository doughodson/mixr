
#ifndef __mixr_graphics_Cursor_H__
#define __mixr_graphics_Cursor_H__

#include "mixr/graphics/Field.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Cursor
//
// Factory name: Cursor
//------------------------------------------------------------------------------
class Cursor : public Field
{
    DECLARE_SUBCLASS(Cursor, Field)

public:
   Cursor();
   virtual void updateData(const double dt = 0.0) override;
};

}
}

#endif
