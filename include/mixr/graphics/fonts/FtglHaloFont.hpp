
#ifndef __mixr_graphics_FtglHaloFont_HPP__
#define __mixr_graphics_FtglHaloFont_HPP__

#include "AbstractFtglFont.hpp"

class FTOutlineFont;

namespace mixr {
namespace base { class Color; class Number; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: FtglHaloFont
// Description: Polygon font that also draws a halo around the font
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: FTGLHaloFonts
// Slots:
//   halocolor      <Color>    ! Font halo color (default = nullptr)
//   linewidth      <Number>   ! Font line width (default = 1)
//------------------------------------------------------------------------------
class FtglHaloFont final: public AbstractFtglFont
{
    DECLARE_SUBCLASS(FtglHaloFont, AbstractFtglFont)

public:
    FtglHaloFont();

    // outputs n characters of txt at coords (x,y). Vertically if vf == true.
    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) final;
    // outputs n characters of txt at the current position. Vertically if vf == true.
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) final;

    base::Color* getHaloColor()           { return haloColor; }
    float getHaloLineWidth()              { return linewidth; }
    FTOutlineFont* getOutline()           { return outline; }

private:
    void loadFont() final;

    FTOutlineFont* outline{};
    base::Color* haloColor{};
    float linewidth{1.0};

private:
    // slot table helper methods
    bool setSlotHaloColor(base::Color*);
    bool setSlotLinewidth(const base::Number* const);
};

}
}

#endif
