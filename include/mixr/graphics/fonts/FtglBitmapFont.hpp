
#ifndef __mixr_graphics_FtglBitmapFont_H__
#define __mixr_graphics_FtglBitmapFont_H__

#include "AbstractFtglFont.hpp"

//#define FTGL_2_1_2  // version 2.1.2 or earlier

//#ifdef FTGL_2_1_2
//class FTGLOutlineFont;
//#else
//#include <FTGL/ftgl.h>
//#endif

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: FtglBitmapFont
//
// Description: Bitmap Font
// Factory name: FTGLBitmapFonts
//
// Public member functions:
//   outputText(double x, double y, char* txt, int n, bool vf)
//   outputText(double x, double y, char* txt, int n)
//   -- Outputs n characters of txt at coords (x,y). Vertically if vf == true.
//
//   outputText(char* txt, int n, bool vf)
//   outputText(char* Txt, int n)
//   -- Outputs n characters of txt at the current position. Vertically if vf == true.
//
//   loadFont()
//   -- Loads the font.
//------------------------------------------------------------------------------
class FtglBitmapFont : public AbstractFtglFont
{
    DECLARE_SUBCLASS(FtglBitmapFont, AbstractFtglFont)

public:
    FtglBitmapFont();

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void loadFont() override;
};

}
}

#endif
