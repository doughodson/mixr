
#include "mixr/graphics/readouts/AsciiText.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/String.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(AsciiText, "AsciiText")
EMPTY_COPYDATA(AsciiText)
EMPTY_DELETEDATA(AsciiText)

BEGIN_SLOTTABLE(AsciiText)
   "text",                  // text string
END_SLOTTABLE(AsciiText)

BEGIN_SLOT_MAP(AsciiText)
   ON_SLOT(1,setSlotTextString, base::String)
   ON_SLOT(1,setSlotTextList,   base::List)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(AsciiText)
   ON_EVENT_OBJ(UPDATE_VALUE, setSlotTextString, base::String)
END_EVENT_HANDLER()

AsciiText::AsciiText()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// input mode functions --
//   filterInputEvent() -- Filter input events using a template character (tc)
//   isValidInputPosition() -- Makes sure the position is valid for input
//------------------------------------------------------------------------------
char AsciiText::filterInputEvent(const int event, const int)
{
   return static_cast<char>(event); // All characters are valid for AsciiText fields
}

bool AsciiText::isValidInputPosition(const int)
{
   return true;     // AsciiText fields do not have template characters
}

// takes in a base::String and sets it
bool AsciiText::setSlotTextString(const base::String* const stsobj)
{
   bool ok {true};
   if (stsobj != nullptr) {
      if (width() == 0) width(stsobj->len());
      setText(*stsobj);
   }
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "AsciiText::setTextString: \"text\" must be a string or a list of (ASCII) numbers!" << std::endl;
      }
      ok = false;
   }
   return ok;
}

// takes in alist of ascii numbers
bool AsciiText::setSlotTextList(const base::List* const stlobj)
{
   bool ok {true};
   if (stlobj != nullptr) {
      float values[256] {};
      int n {static_cast<int>(stlobj->getNumberList(values, 256))};
      if (n > 0) {
         char cbuf[258] {};
         int j {};
         for (j = 0; j < n; j++) {
            cbuf[j] = char(values[j]);
         }
         cbuf[j] = '\0';
         if (width() == 0) width(j);
         setText(cbuf);
         ok = true;
      } else {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "AsciiText::setTextList: \"text\" must be a string or a list of (ASCII) numbers!" << std::endl;
         }
         ok = false;
      }
   }
   return ok;
}

}
}
