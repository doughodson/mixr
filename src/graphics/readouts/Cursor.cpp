
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
   setFlashRate(1.0f);
}

void Cursor::updateData(const double dt)
{
   int ln = 0;
   int cp = 0;
   if (getDisplay()->focus() != nullptr) {
      setVisibility( getDisplay()->focus()->cursor(&ln, &cp) );
   }
   else {
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
