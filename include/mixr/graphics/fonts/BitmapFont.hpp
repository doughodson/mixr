
#ifndef __mixr_graphics_BitmapFont_H__
#define __mixr_graphics_BitmapFont_H__

#include "AbstractFont.hpp"

namespace mixr {
namespace base { class Boolean; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: BitmapFont
// Description: Creates and load bit map fonts
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: BitmapFont
// Slots:
//   reverse   <Boolean>    ! Reverse video (default: false)
//------------------------------------------------------------------------------
class BitmapFont final: public AbstractFont
{
    DECLARE_SUBCLASS(BitmapFont, AbstractFont)

public:
    BitmapFont();

    // output n characters of txt at (x,y). Output vertically if vf == true.
    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) final;
    // output n characters of txt at the current position. Output vertically if vf == true.
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) final;

private:
    void loadFont() final;

    // loader support functions
    GLubyte* loadTypeFace(const GLint index, const GLenum reverse);
    static void reverseBitmapOrder(GLubyte* bitmap, unsigned int numBitmapBytes, unsigned int numBytesWide);
    static GLubyte reverseByteOrder(GLubyte byte);

    bool reverse{};                // Reverse the font
    const char** fontMap{};        // Font map (ASCII code to file name mapping)
    unsigned int numFonts{};       // Number of fonts in the map

    // Default fontMap
    static const int defaultNumFonts;
    static const char** defaultFontMap;

private:
    // slot table helper methods
    bool setSlotReverse(const base::Boolean* const);
};

}
}

#endif
