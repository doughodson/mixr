
#include "mixr/graphics/readouts/Rotary2.hpp"

#include "mixr/graphics/Graphic.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Rotary2, "Rotary2")
EMPTY_SLOTTABLE(Rotary2)
EMPTY_COPYDATA(Rotary2)
EMPTY_DELETEDATA(Rotary2)

BEGIN_EVENT_HANDLER(Rotary2)
   ON_EVENT_OBJ(SELECT, onSelect, base::Boolean)
   ON_EVENT_OBJ(SELECT, onSelect, base::Integer)
END_EVENT_HANDLER()

Rotary2::Rotary2()
{
   STANDARD_CONSTRUCTOR()

   const auto p = new base::Integer(1);    // default rotary item
   setSelectionName(p);
   p->unref();
}

// Rotary - we do this "pre-draw" of all of our possible selections, that will
// eliminate the "flicker" on the first selection of the item, because we will have
// already drawn the item one time before (here).
void Rotary2::draw()
{
   if (preDrawSelectList) {
      int start{1};
      base::Pair* p{findByIndex(start)};
      while (p != nullptr) {
         const auto g = dynamic_cast<graphics::Graphic*>(p->object());
         if (g != nullptr) g->draw();
         p = findByIndex(++start);
      }
      preDrawSelectList = false;
   }

   BaseClass::draw();
}

// onSelect() - Macro function for Rotary2
bool Rotary2::onSelect(const base::Boolean* const x)
{
   if (x != nullptr) {
      if (x->asBool()){
         //if true, select the second component
         base::Integer two(2);
         select(&two);
      } else {
         //if false, select the first component
         base::Integer one(1);
         select(&one);
      }
   }
   return true;
}

// onSelect() - Macro function for Rotary2
bool Rotary2::onSelect(const base::Integer* const x)
{
   select(x);
   return true;
}

}
}
