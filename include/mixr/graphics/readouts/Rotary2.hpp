
#ifndef __mixr_graphics_Rotary2_H__
#define __mixr_graphics_Rotary2_H__

#include "Rotary.hpp"

namespace mixr {
namespace base { class Object; class Number; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: Rotary2
//
// Description: Binary rotary
//
// Factory name: Rotary2
//
//------------------------------------------------------------------------------
class Rotary2 : public Rotary
{
    DECLARE_SUBCLASS(Rotary2, Rotary)

public:
   Rotary2();
   bool event(const int key, base::Object* const obj = nullptr) override;

   // event handler methods
   virtual bool onSelect(const base::Number* const osobj);
};

}
}

#endif
