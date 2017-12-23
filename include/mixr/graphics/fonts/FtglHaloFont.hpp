
#ifndef __mixr_graphics_FtglHaloFont_H__
#define __mixr_graphics_FtglHaloFont_H__

#include "AbstractFtglFont.hpp"

class FTOutlineFont;

namespace mixr {
namespace base { class Color; class Number; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: FtglHaloFont
//
// Description: Polygon font that also draws a halo around the font
// Factory name: FTGLHaloFonts
// Slots:
//   halocolor      <Color>    ! Font halo color (default = 0)
//   linewidth      <Number>   ! Font line width (default = 1)
//
// Public member functions:
//    outputText(double x, double y, char* txt, int n, bool vf)
//    outputText(double x, double y, char* txt, int n)
//       Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//    outputText(char* txt, int n, bool vf)
//    outputText(char* txt, int n)
//       Outputs n characters of txt at the current position. Vertically if vf == true.
//
// loadFont()
//    Loads the font.
//------------------------------------------------------------------------------
class FtglHaloFont : public AbstractFtglFont
{
    DECLARE_SUBCLASS(FtglHaloFont, AbstractFtglFont)

public:
    FtglHaloFont();

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void loadFont() override;

    base::Color* getHaloColor()           { return haloColor; }
    float getHaloLineWidth()              { return linewidth; }
    FTOutlineFont* getOutline()           { return outline; }

private:
    FTOutlineFont* outline {};
    base::Color* haloColor {};
    float linewidth {1.0};

private:
    // slot table helper methods
    bool setSlotHaloColor(base::Color*);
    bool setSlotLinewidth(const base::Number* const);
};

}
}

#endif
