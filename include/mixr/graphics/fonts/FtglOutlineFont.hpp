
#ifndef __mixr_graphics_FtglOutlineFont_H__
#define __mixr_graphics_FtglOutlineFont_H__

#include "AbstractFtglFont.hpp"

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class:  FtglOutlineFont
//
// Description: Outlined Font
// Factory name: FTGLOutlineFonts
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
class FtglOutlineFont : public AbstractFtglFont
{
    DECLARE_SUBCLASS(FtglOutlineFont, AbstractFtglFont)

public:
    FtglOutlineFont();

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void loadFont() override;
};

}
}

#endif
