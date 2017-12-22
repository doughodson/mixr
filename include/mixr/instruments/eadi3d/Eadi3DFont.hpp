
#ifndef __mixr_instruments_Eadi3DFont_H__
#define __mixr_instruments_Eadi3DFont_H__

#include "mixr/graphics/fonts/AbstractFont.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: Eadi3DFont
//
// Description: Eadi3D font
//
// public methods (member functions):
//      (All of the classes derived from AbstractFont have the same public methods.)
//
//      static GLenum createEadi3DFont(GLuint fontBase)
//          Produces the stroke font map.
//
//------------------------------------------------------------------------------
class Eadi3DFont : public graphics::AbstractFont
{
    DECLARE_SUBCLASS(Eadi3DFont, graphics::AbstractFont)

public:
    Eadi3DFont();

    static GLenum createEadi3DFont(GLuint);

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void loadFont() override;
};

}
}

#endif
