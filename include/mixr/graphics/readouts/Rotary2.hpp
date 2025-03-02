
#ifndef __mixr_graphics_Rotary2_HPP__
#define __mixr_graphics_Rotary2_HPP__

#include "mixr/graphics/readouts/IReadout.hpp"

namespace mixr {
namespace base { class Boolean; class Integer; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: Rotary2
// Description: Binary rotary
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Rotary2
// Slots: none
//------------------------------------------------------------------------------
// Events:
//    SELECT    <Boolean>   !
//    SELECT    <Integer>   !
//------------------------------------------------------------------------------
class Rotary2 final: public IReadout
{
    DECLARE_SUBCLASS(Rotary2, IReadout)

public:
   Rotary2();

   void draw() final;
   bool event(const int key, base::IObject* const obj = nullptr) final;

   // event handler methods
   bool onSelect(const base::Boolean* const);
   bool onSelect(const base::Integer* const);

private:
    // this flag tells us our components need to be pre-drawn (to avoid flicker)
    bool preDrawSelectList{true};
};

}
}

#endif
