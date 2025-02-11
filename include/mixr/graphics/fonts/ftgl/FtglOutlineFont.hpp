
#ifndef __mixr_graphics_FtglOutlineFont_HPP__
#define __mixr_graphics_FtglOutlineFont_HPP__

#include "IFtglFont.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class:  FtglOutlineFont
// Description: Outlined Font
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: FTGLOutlineFonts
// Slots: none
//------------------------------------------------------------------------------
class FtglOutlineFont final: public IFtglFont
{
    DECLARE_SUBCLASS(FtglOutlineFont, IFtglFont)

public:
    FtglOutlineFont();

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
