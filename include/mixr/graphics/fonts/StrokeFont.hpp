
#ifndef __mixr_graphics_StrokeFont_H__
#define __mixr_graphics_StrokeFont_H__

#include "AbstractFont.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: StrokeFont
// Description: Modified version of SGI's stroke font
// Factory name: StrokeFont
//
// public methods (member functions):
//      (All of the classes derived from Font have the same public methods.)
//
//      static GLenum createStrokeFont(GLuint fontBase)
//          Produces the stroke font map.
//
//------------------------------------------------------------------------------
class StrokeFont : public AbstractFont
{
    DECLARE_SUBCLASS(StrokeFont, AbstractFont)

public:
    StrokeFont();

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void loadFont() override;

    static GLenum createStrokeFont(GLuint fontBase);
};

}
}

#endif
