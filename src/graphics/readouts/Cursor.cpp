
#include "mixr/graphics/readouts/Cursor.hpp"

#include "mixr/graphics/Display.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Cursor, "Cursor")
EMPTY_SLOTTABLE(Cursor)
EMPTY_COPYDATA(Cursor)
EMPTY_DELETEDATA(Cursor)

Cursor::Cursor()
{
   STANDARD_CONSTRUCTOR()

   width(1);
   setText(" ");
   setDisplayMode(reversed);
   setFlashRate(1.0);
}

void Cursor::updateData(const double dt)
{
   int ln {};
   int cp {};
   if (getDisplay()->focus() != nullptr) {
      setVisibility( getDisplay()->focus()->cursor(&ln, &cp) );
   } else {
      setVisibility(false);
   }

   if (isVisible()) {
      line(ln);
      column(cp);
      BaseClass::updateData(dt);
   }
}

}
}
