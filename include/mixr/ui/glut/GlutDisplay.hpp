
#ifndef __mixr_ui_glut_GlutDisplay_HPP__
#define __mixr_ui_glut_GlutDisplay_HPP__

#include "mixr/ui/glut/IGlutDisplay.hpp"

namespace mixr {
namespace glut {

//------------------------------------------------------------------------------
// Class: GlutDisplay
//
// Description: Concrete class that manages the Display as a GLUT Window
// Factory name: GlutDisplay
//------------------------------------------------------------------------------
class GlutDisplay final: public IGlutDisplay
{
   DECLARE_SUBCLASS(GlutDisplay, IGlutDisplay)

public:
   GlutDisplay();
};

}
}

#endif
