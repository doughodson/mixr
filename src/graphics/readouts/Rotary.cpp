
#include "mixr/graphics/readouts/Rotary.hpp"

#include "mixr/graphics/Graphic.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Rotary, "Rotary")
EMPTY_SLOTTABLE(Rotary)
EMPTY_COPYDATA(Rotary)
EMPTY_DELETEDATA(Rotary)

Rotary::Rotary()
{
   STANDARD_CONSTRUCTOR()

   const auto p = new base::Integer(1);    // default rotary item
   setSelectionName(p);
   p->unref();
}

// Rotary - we do this "pre-draw" of all of our possible selections, that will
// eliminate the "flicker" on the first selection of the item, because we will have
// already drawn the item one time before (here).
void Rotary::draw()
{
   if (preDrawSelectList) {
      int start {1};
      base::Pair* p {findByIndex(start)};
      while (p != nullptr) {
         const auto g = dynamic_cast<graphics::Graphic*>(p->object());
         if (g != nullptr) g->draw();
         p = findByIndex(++start);
      }
      preDrawSelectList = false;
   }

   BaseClass::draw();
}

}
}
