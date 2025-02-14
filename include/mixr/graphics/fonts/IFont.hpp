
#ifndef __mixr_graphics_IFont_HPP__
#define __mixr_graphics_IFont_HPP__

#include "mixr/base/Object.hpp"

#include "mixr/base/util/platform_api.hpp"
#include <GL/gl.h>

#include <string>

namespace mixr {
namespace base { class Integer; class INumber; class List; class String; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: IFont
// Description: Interface that defines base functionality for all font classes
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: IFont
// Slots:
//      fontWidth        <INumber>  ! Font width (default: 1)
//      fontHeight       <INumber>  ! Font height (default: 1)
//      fontPosition     <List>     ! Upper/Left origin of the text field (default: 0, 0)
//      bitmapWidth      <Integer>  ! Width of bit map font (default: 1)
//      bitmapHeight     <Integer>  ! Height of bit map font (default: 1)
//      path             <String>   ! Path to the font directory (default: nullptr)
//      file             <String>   ! FTGL Font file name - will be altered in each instance of FTGLFont (default: nullptr)
//      lut              <List>     ! Lookup Table (default: 0)
//      characterSpacing <INumber>  ! spacing for each character (default: 0)
//      lineSpacing      <INumber>  ! spacing for each line (default: 0)
//------------------------------------------------------------------------------
class IFont : public base::Object
{
    DECLARE_SUBCLASS(IFont, base::Object)

public:
    static const std::size_t MAX_MESSAGE_LENGTH{256};  // Max length of character buffers

public:
    IFont();

    // Outputs the text at position (x, y).
    // When 'vf' is true, text is drawn vertically
    virtual void outputText(const double x, const double y, const char* txt, const int n, const bool vf = false, const bool rf = false) =0;
    // Outputs the text with the (ln, cp) pair (line, column). (Current Position)
    // When 'vf' is true, text is drawn vertically
    virtual void outputText(const char* txt, const int n, const bool vf = false, const bool rf = false) =0;
    // computes the (X, Y) position of the (ln, cp) pair (line, column)
    virtual void position(const int ln, const int cp, GLdouble& px, GLdouble& py) const;

    // sets the upper left corner
    void setTextOrigin(const GLdouble x, const GLdouble y);

    // set/get character spacing
    void setCharacterSpacing(const double v)            { charSpacing = v; }
    double getCharacterSpacing() const                  { return charSpacing; }

    // set/get line spacing
    void setLineSpacing(const double v)                 { lineSpacing = v; }
    double getLineSpacing() const                       { return lineSpacing; }

    void setFontWidth(const GLdouble v)                 { fWidth = v; }
    GLdouble getFontWidth() const                       { return fWidth; }

    void setFontHeight(const GLdouble v)                { fHeight = v; }
    GLdouble getFontHeight() const                      { return fHeight; }

    void setBase(const GLuint nb)                       { b = nb; }
    GLuint getBase() const                              { return b; }

    void setBitmapWidth(const GLuint v)                 { bWidth = v; }
    GLuint getBitmapWidth() const                       { return bWidth; }

    void setBitmapHeight(const GLuint v)                { bHeight = v; }
    GLuint getBitmapHeight() const                      { return bHeight; }

    // returns true if the font is loaded, else false
    bool isLoaded() const                               { return loaded; }
    // returns true if the font is not loaded, else false
    bool isNotLoaded() const                            { return !loaded; }

    // returns pLUT (string pointer to the Look-Up-Table <character translations>)
    const unsigned char* lut() const                    { return pLUT; }

    // returns true if the font is an OpenGL TrueType Font (FTGL), else false
    bool isFTGL() const                                 { return (pFTGL != nullptr); }
    // returns a pointer to an OpenGL Freetype2 TrueType Font (FTGL)
    void* FTGL()                                        { return pFTGL; }

    // returns the font directory and filename
    const std::string& fontDirectory()                  { return fontPath; }
    const std::string& filename()                       { return fontFile; }

public:
    // exception: thrown by method outputText() when the font map is not valid and
    // couldn't be loaded.
    class ExpInvalidFont : public base::Object::Exception {
        public:
            ExpInvalidFont() : Exception() {}
            const char* getDescription() const override     { return "font is invalid"; }
    };

protected:

    static const std::size_t MSG_BUF_LEN{MAX_MESSAGE_LENGTH+1}; // Max length of character buffers
    // transfers n characters of inp to outp and returns the size of outp
    int xferChars(char* const outp, const std::size_t BUF_SIZE, const char* const inp, const unsigned int n) const;

    GLdouble leftSide{}, topSide{};             // Origin: upper left corner of ln=1, cp=1

    // sets the pFTGL font pointer to p
    void ftgl(void* p)                                  { pFTGL = p; }
    void setFontLoaded()                                { loaded = true; }

private:
    void initData();
    virtual void loadFont() =0;

    static const int LUT_SIZE{256};
    GLdouble  fWidth{}, fHeight{};       // Font size
    GLuint    bWidth{}, bHeight{};       // Bitmap size
    GLuint    b{};                       // Font call list base
    unsigned char* pLUT{};               // Look-Up-Table (character translations)
    void*     pFTGL{};                   // OpenGL TrueType Font (FTGL)
    bool      loaded{};                  // Font has been loaded
    double charSpacing{};                // holds our character spacing
    double lineSpacing{};                // holds our line spacing

    std::string fontPath;                // Path to font directory (FTGL & Bitmap fonts)
    std::string fontFile;                // Font filename (FTGL)

private:
    // slot table helper methods
    bool setSlotFontWidth(const base::INumber* const);
    bool setSlotFontHeight(const base::INumber* const);
    bool setSlotFontPosition(const base::List* const);
    bool setSlotBitmapWidth(const base::Integer* const);
    bool setSlotBitmapHeight(const base::Integer* const);
    bool setSlotFontPath(const base::String* const);
    bool setSlotFTGLFontFileName(const base::String* const);
    bool setSlotLookupTable(const base::List* const);
    bool setSlotCharacterSpacing(const base::INumber* const);
    bool setSlotLineSpacing(const base::INumber* const);
};

}
}

#endif
