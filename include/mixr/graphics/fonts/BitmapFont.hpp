
#ifndef __mixr_graphics_BitmapFont_H__
#define __mixr_graphics_BitmapFont_H__

#include "AbstractFont.hpp"

namespace mixr {
namespace base { class Number; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: BitmapFont
//
// Description: Creates and load bit map fonts
//
// Factory name: BitmapFont
// Slots:
//   reverse   <Number>    ! Reverse video (default: false)
//
// public methods (member functions):
//      (All of the classes derived from Font have the same public methods.)
//      (See font.hpp for descriptions of those functions.)
//
//   outputText(double x, double y, char* txt, int n, bool vf)
//   -- Output n characters of txt at (x,y). Output vertically if vf == true.
//
//   outputText(char* txt, int n, bool vf)
//   -- Output n characters of txt at the current position. Output vertically if vf == true.
//
//   loadFont()
//   -- Loads the font.
//
//   setReverse(Number* rnumber)
//   -- Sets the font to reverse if rnumber != 0.
//
//------------------------------------------------------------------------------
class BitmapFont : public AbstractFont
{
    DECLARE_SUBCLASS(BitmapFont, AbstractFont)

public:
    BitmapFont();

    void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) override;
    void loadFont() override;

private:
    // Loader support functions
    GLubyte* loadTypeFace(const GLint index, const GLenum reverse);
    static void reverseBitmapOrder(GLubyte* bitmap, unsigned int numBitmapBytes, unsigned int numBytesWide);
    static GLubyte reverseByteOrder(GLubyte byte);

    bool reverse {};                // Reverse the font
    const char** fontMap {};        // Font map (ASCII code to file name mapping)
    unsigned int numFonts {};       // Number of fonts in the map

    // Default fontMap
    static const char** defaultFontMap;
    static const unsigned int defaultNumFonts;

private:
    // slot table helper methods
    bool setSlotReverse(const base::Number* const);
};

}
}

#endif
