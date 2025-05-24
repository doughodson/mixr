
#include "mixr/graphics/readouts/AsciiText.hpp"

#include "mixr/base/IList.hpp"
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
   ON_SLOT(1,setSlotTextList,   base::IList)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(AsciiText)
   ON_EVENT_OBJ(UPDATE_VALUE, setSlotTextString, base::String)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// input mode functions --
//   filterInputEvent() -- Filter input events using a template character (tc)
//   isValidInputPosition() -- Makes sure the position is valid for input
//------------------------------------------------------------------------------
char AsciiText::filterInputEvent(const int event, const char)
{
   return static_cast<char>(event); // All characters are valid for AsciiText fields
}

bool AsciiText::isValidInputPosition(const int)
{
   return true;     // AsciiText fields do not have template characters
}

// takes in a base::String and sets it
bool AsciiText::setSlotTextString(const base::String* const x)
{
   if (width() == 0) width(x->len());
   setText((*x).c_str());
   return true;
}

// takes in a list of ascii numbers
bool AsciiText::setSlotTextList(const base::IList* const x)
{
   bool ok {true};
   float values[256] {};
   std::size_t n {x->getNumberList(values, 256)};
   if (n > 0) {
      char cbuf[258] {};
      std::size_t j {};
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
   return ok;
}

}
}
