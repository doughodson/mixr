
#ifndef __mixr_graphics_BooleanText_H__
#define __mixr_graphics_BooleanText_H__

#include "mixr/graphics/readouts/AbstractReadout.hpp"

namespace mixr {
namespace base { class Object; class List; class String; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: BooleanText
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: text
// Slots:
//    text   <String>     ! Text String (default: " ")
//    text   <List>       ! Text List
//------------------------------------------------------------------------------

class BooleanText final: public AbstractReadout
{
    DECLARE_SUBCLASS(BooleanText, AbstractReadout)

public:
   BooleanText()                  { STANDARD_CONSTRUCTOR() }

   char filterInputEvent(const int event, const int tc) final;
   // returns true if tc is a valid input position
   bool isValidInputPosition(const int) final;

   bool event(const int key, base::Object* const obj = nullptr) final;

   // returns true if the text string was set to stsobj
   bool setBooleanText(const base::Boolean* const stsobj);
};

}
}

#endif
