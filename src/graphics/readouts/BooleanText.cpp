
#include "mixr/graphics/readouts/BooleanText.hpp"

#include "mixr/base/numeric/Boolean.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(BooleanText, "BooleanText")
EMPTY_COPYDATA(BooleanText)
EMPTY_DELETEDATA(BooleanText)
EMPTY_SLOTTABLE(BooleanText)

BEGIN_EVENT_HANDLER(BooleanText)
   ON_EVENT_OBJ(UPDATE_VALUE, setBooleanText, base::Boolean)
END_EVENT_HANDLER()

//------------------------------------------------------------------------------
// input mode functions --
//   filterInputEvent() -- Filter input events using a template character (tc)
//   isValidInputPosition() -- Makes sure the position is valid for input
//------------------------------------------------------------------------------
char BooleanText::filterInputEvent(const int event, const int)
{
   return static_cast<char>(event); // All characters are valid for AsciiText fields
}

bool BooleanText::isValidInputPosition(const int)
{
   return true;     // AsciiText fields do not have template characters
}

// takes in a base::Boolean and sets it
bool BooleanText::setBooleanText(const base::Boolean* const x)
{
   bool ok {true};
   if (x != nullptr) {
      if (x->asBool()) {
         setText("True");
      } else {
         setText("False");
      }
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "BooleanText::setBooleanText: Invalid Boolean" << std::endl;
      }
      ok = false;
   }
   return ok;
}

}
}
