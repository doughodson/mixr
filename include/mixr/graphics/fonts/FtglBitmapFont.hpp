
#ifndef __mixr_graphics_FtglBitmapFont_HPP__
#define __mixr_graphics_FtglBitmapFont_HPP__

#include "AbstractFtglFont.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: FtglBitmapFont
// Description: Bitmap Font
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: FTGLBitmapFonts
// Slots: none
//------------------------------------------------------------------------------
class FtglBitmapFont final: public AbstractFtglFont
{
    DECLARE_SUBCLASS(FtglBitmapFont, AbstractFtglFont)

public:
    FtglBitmapFont();

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
