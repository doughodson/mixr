
#ifndef __mixr_graphics_FtglExtrdFont_H__
#define __mixr_graphics_FtglExtrdFont_H__

#include "AbstractFtglFont.hpp"

namespace mixr {
namespace base { class Number; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: FtglExtrdFont
// Description: Creates a Freetype Extruded Font type
// Factory name: FTGLExtrdFonts
// Slots:
//   depth         <Number>    ! Depth (default: 5.0f)
//
// public member functions:
//   setDepth(const base::Number* const newDepth)
//   -- sets the depth of the extruded font
//
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
class FtglExtrdFont : public AbstractFtglFont
{
    DECLARE_SUBCLASS(FtglExtrdFont, AbstractFtglFont)

public:
    FtglExtrdFont();

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void loadFont() override;

private:
    static const float DEFAULT_DEPTH;
    float depth{DEFAULT_DEPTH};        // depth of the extruded font (for 3D purposes)

private:
    // slot table helper methods
    bool setSlotDepth(const base::Number* const);
};

}
}

#endif
