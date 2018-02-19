
#ifndef __mixr_graphics_FtglExtrdFont_H__
#define __mixr_graphics_FtglExtrdFont_H__

#include "AbstractFtglFont.hpp"

namespace mixr {
namespace base { class Number; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: FtglExtrdFont
// Description: Creates a Freetype Extruded Font type
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: FTGLExtrdFonts
// Slots:
//   depth         <Number>    ! Depth (default: 5.0)
//------------------------------------------------------------------------------
class FtglExtrdFont final: public AbstractFtglFont
{
    DECLARE_SUBCLASS(FtglExtrdFont, AbstractFtglFont)

public:
    FtglExtrdFont();

    // outputs n characters of txt at coords (x,y). Vertically if vf == true.
    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) final;
    // outputs n characters of txt at the current position. Vertically if vf == true.
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) final;

private:
    void loadFont() final;

    static const float DEFAULT_DEPTH;
    float depth{DEFAULT_DEPTH};        // depth of the extruded font (for 3D purposes)

private:
    // slot table helper methods
    // sets the depth of the extruded font
    bool setSlotDepth(const base::Number* const);
};

}
}

#endif
