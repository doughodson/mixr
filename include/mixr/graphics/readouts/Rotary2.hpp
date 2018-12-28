
#ifndef __mixr_graphics_Rotary2_H__
#define __mixr_graphics_Rotary2_H__

#include "mixr/graphics/readouts/AbstractReadout.hpp"

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
class Rotary2 final: public AbstractReadout
{
    DECLARE_SUBCLASS(Rotary2, AbstractReadout)

public:
   Rotary2();

   void draw() final;
   bool event(const int key, base::Object* const obj = nullptr) final;

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
