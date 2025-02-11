
#ifndef __mixr_graphics_BooleanText_HPP__
#define __mixr_graphics_BooleanText_HPP__

#include "mixr/graphics/readouts/IReadout.hpp"

namespace mixr {
namespace base { class Object; class List; class String; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: BooleanText
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: BooleanText
// Slots: none
//------------------------------------------------------------------------------

class BooleanText final: public IReadout
{
    DECLARE_SUBCLASS(BooleanText, IReadout)

public:
   BooleanText()                  { STANDARD_CONSTRUCTOR() }

   char filterInputEvent(const int event, const char) final;
   // returns true if tc is a valid input position
   bool isValidInputPosition(const int) final;

   bool event(const int key, base::Object* const obj = nullptr) final;

   // returns true if the text string was set to stsobj
   bool setBooleanText(const base::Boolean* const);
};

}
}

#endif
