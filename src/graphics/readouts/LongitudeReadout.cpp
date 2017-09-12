
#include "mixr/graphics/readouts/LongitudeReadout.hpp"

#include "mixr/base/util/str_utils.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(LongitudeReadout, "LongitudeReadout")
EMPTY_SLOTTABLE(LongitudeReadout)
EMPTY_COPYDATA(LongitudeReadout)
EMPTY_DELETEDATA(LongitudeReadout)

LongitudeReadout::LongitudeReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format, FORMAT_LENGTH, "%+04d@%04.1f");
   tmode = DirMode::ddmm;
   plusChar = 'E';
   minusChar = 'W';
   postSign = false;
}


//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char LongitudeReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '+') {
      // sign keys
      if (event == '4')
         return 'W';
      else if (event == '6')
         return 'E';
      else
         return '\0';
   }
   else {
      return BaseClass::filterInputEvent(event, tc);
   }
}

}
}
