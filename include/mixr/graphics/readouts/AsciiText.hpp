
#ifndef __mixr_graphics_AsciiText_H__
#define __mixr_graphics_AsciiText_H__

#include "mixr/graphics/readouts/AbstractReadout.hpp"

namespace mixr {
namespace base { class Object; class List; class String; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: AsciiText
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: text
// Slots:
//    text   <String>     ! Text String (default: " ")
//    text   <List>       ! Text List
//------------------------------------------------------------------------------

class AsciiText final: public AbstractReadout
{
    DECLARE_SUBCLASS(AsciiText, AbstractReadout)

public:
   AsciiText()                  { STANDARD_CONSTRUCTOR() }

   char filterInputEvent(const int event, const char) final;
   // returns true if tc is a valid input position
   bool isValidInputPosition(const int) final;

   bool event(const int key, base::Object* const obj = nullptr) final;

   // returns true if the text string was set to stsobj
   bool setSlotTextString(const base::String* const stsobj);

   // returns true if the text list was set to stlobj
   bool setSlotTextList(const base::List* const stlobj);
};

}
}

#endif
