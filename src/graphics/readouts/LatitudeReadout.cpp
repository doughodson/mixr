
#include "mixr/graphics/readouts/LatitudeReadout.hpp"

#include "mixr/base/util/str_utils.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(LatitudeReadout, "LatitudeReadout")
EMPTY_SLOTTABLE(LatitudeReadout)
EMPTY_COPYDATA(LatitudeReadout)
EMPTY_DELETEDATA(LatitudeReadout)

LatitudeReadout::LatitudeReadout()
{
   STANDARD_CONSTRUCTOR()

   base::utStrcpy(format, FORMAT_LENGTH, "%+03d@%04.1f");
   tmode = DirMode::ddmm;
   plusChar = 'N';
   minusChar = 'S';
   postSign = false;
}

//------------------------------------------------------------------------------
// input mode function --
//   filterInputEvent() -- Filter input events using a template character (tc)
//------------------------------------------------------------------------------
char LatitudeReadout::filterInputEvent(const int event, const int tc)
{
   if (tc == '+') {
      // sign keys
      if (event == '8')
         return 'S';
      else if (event == '2')
         return 'N';
      else
         return '\0';
   }
   else {
      return BaseClass::filterInputEvent(event, tc);
   }
}

}
}
