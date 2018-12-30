
#ifndef __mixr_graphics_FtglPixmapFont_HPP__
#define __mixr_graphics_FtglPixmapFont_HPP__

#include "AbstractFtglFont.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: FtglPixmapFont
// Description: Pixmapped Font
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: FTGLPixmapFonts
// Slots: none
//------------------------------------------------------------------------------
class FtglPixmapFont final: public AbstractFtglFont
{
    DECLARE_SUBCLASS(FtglPixmapFont, AbstractFtglFont)

public:
    FtglPixmapFont();

    // outputs n characters of txt at coords (x,y). Vertically if vf == true.
    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) final;
    // outputs n characters of txt at the current position. Vertically if vf == true.
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) final;

private:
    void loadFont() final;
};

}
}

#endif
