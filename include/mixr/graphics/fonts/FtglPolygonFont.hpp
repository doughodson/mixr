
#ifndef __mixr_graphics_FtglPolygonFont_HPP__
#define __mixr_graphics_FtglPolygonFont_HPP__

#include "IFtglFont.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: FtglPolygonFont
// Description: Polygon Font
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: FTGLPolygonFonts
// Slots: none
//------------------------------------------------------------------------------
class FtglPolygonFont final: public IFtglFont
{
    DECLARE_SUBCLASS(FtglPolygonFont, IFtglFont)

public:
    FtglPolygonFont();

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
