
#ifndef __mixr_graphics_AsciiText_HPP__
#define __mixr_graphics_AsciiText_HPP__

#include "mixr/graphics/readouts/IReadout.hpp"

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

class AsciiText final: public IReadout
{
    DECLARE_SUBCLASS(AsciiText, IReadout)

public:
   AsciiText()                  { STANDARD_CONSTRUCTOR() }

   char filterInputEvent(const int event, const char) final;
   // returns true if tc is a valid input position
   bool isValidInputPosition(const int) final;

   bool event(const int key, base::Object* const obj = nullptr) final;

private:
   // slot table helper methods
   bool setSlotTextString(const base::String* const);
   bool setSlotTextList(const base::List* const);
};

}
}

#endif
