
#ifndef __mixr_graphics_StrokeFont_HPP__
#define __mixr_graphics_StrokeFont_HPP__

#include "AbstractFont.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: StrokeFont
// Description: Modified version of SGI's stroke font
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: StrokeFont
// Slots: none
//------------------------------------------------------------------------------
class StrokeFont final: public AbstractFont
{
    DECLARE_SUBCLASS(StrokeFont, AbstractFont)

public:
    StrokeFont();

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) final;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) final;

    // creates the stroke font map
    static GLenum createStrokeFont(GLuint fontBase);

private:
    void loadFont() final;
};

}
}

#endif
