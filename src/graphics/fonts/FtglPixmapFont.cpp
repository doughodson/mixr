
#include "mixr/graphics/fonts/FtglPixmapFont.hpp"

#include "mixr/base/util/str_utils.hpp"

#include <iostream>

#include <FTGL/ftgl.h>

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(FtglPixmapFont, "FTGLPixmapFonts")
EMPTY_DELETEDATA(FtglPixmapFont)
EMPTY_SLOTTABLE(FtglPixmapFont)

FtglPixmapFont::FtglPixmapFont()
{
    STANDARD_CONSTRUCTOR();
}

void FtglPixmapFont::copyData(const FtglPixmapFont& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// Pixmap Font functions
// FtglPixmapFont::outputText() -- FTGLPixmapFont output routines
//------------------------------------------------------------------------------
void FtglPixmapFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN] {};
    int nn {xferChars(cbuf,MSG_BUF_LEN,txt,n)};
    if (nn <= 0) return;

    // output the text
    const auto ftgl1 = static_cast<FTGLPixmapFont*>(FTGL());
    if (ftgl1 != nullptr) {
        if (vf) {
            // Vertical text
            char cc[2] {};
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( x, (y - static_cast<float>(i)*getLineSpacing()) );
                ftgl1->Render(cc);
            }
        } else {
            // Normal text
            glRasterPos2d(x,y);
            ftgl1->Render(cbuf);
        }
    } else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FTGLPixmapFont::outputText() - no Pixmap font available" << std::endl;
        }
    }
}

void FtglPixmapFont::outputText(const char* txt, const int n, const bool vf, const bool)
{
    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN] {};
    int nn {xferChars(cbuf,MSG_BUF_LEN,txt,n)};
    if (nn <= 0) return;

    // output the text
    const auto ftgl1 = static_cast<FTGLPixmapFont*>(FTGL());
    if (ftgl1 != nullptr) {
        if (vf) {
            // Vertical text
            char cc[2] {};
            cc[1] = '\0';
            GLdouble rpos[4] {};
            glGetDoublev(GL_CURRENT_RASTER_POSITION, rpos);
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                glRasterPos2d( rpos[0], (rpos[1] - static_cast<float>(i)*getLineSpacing()) );
                ftgl1->Render(cc);
            }
        } else {
            // Normal text
            ftgl1->Render(cbuf);
        }
    } else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglPixmapFont::outputText() - no pixmap font available" << std::endl;
        }
    }
}

void FtglPixmapFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters

    if ( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglPixmapFont::outputText() - no ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const std::size_t FONTPATHNAME_LENGTH {256};
    char fontPathname[FONTPATHNAME_LENGTH] {};
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLPixmapFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());
        ftgl(ftglFont);
        setFontLoaded();
    } else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglPixmapFont::loadFont() - font did not load correctly: file: \"";
            std::cerr << fontPathname << "\"";
            std::cerr << std::endl;
        }
        std::exit(1);
    }
}

}
}
