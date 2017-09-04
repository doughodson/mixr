
#include "mixr/graphics/fonts/FtglBitmapFont.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/util/str_utils.hpp"

#include <iostream>

#include <FTGL/ftgl.h>

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(FtglBitmapFont, "FTGLBitmapFonts")
EMPTY_DELETEDATA(FtglBitmapFont)
EMPTY_SLOTTABLE(FtglBitmapFont)

FtglBitmapFont::FtglBitmapFont()
{
    STANDARD_CONSTRUCTOR();
}

void FtglBitmapFont::copyData(const FtglBitmapFont& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// BitmapFonts functions
// FtglBitmapFont::outputText() -- FTGLBitmapFont output routines
//------------------------------------------------------------------------------
void FtglBitmapFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
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
    int nn {xferChars(cbuf, MSG_BUF_LEN, txt, n)};
    if (nn <= 0) return;

    // output the text
    const auto ftgl1 = static_cast<FTGLBitmapFont*>(FTGL());
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
        }
        else {
            // Normal text
            glRasterPos2d(x,y);
            ftgl1->Render(cbuf);
        }
    }
    else if (isMessageEnabled(MSG_ERROR)) {
        std::cerr << "FtglBitmapFont::outputText() - no Bitmap font available" << std::endl;
    }
}

void FtglBitmapFont::outputText(const char* txt, const int n, const bool vf, const bool)
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
    const int nn {xferChars(cbuf,MSG_BUF_LEN,txt,n)};
    if (nn <= 0) return;

    // output the text
    const auto ftgl1 = static_cast<FTGLBitmapFont*>(FTGL());
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
        }
        else {
            // Normal text
            ftgl1->Render(cbuf);
        }
    }
    else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglBitmapFont::outputText() - no Bitmap font available" << std::endl;
        }
    }
}

// Load FtglBitmapFont
void FtglBitmapFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters

    if ( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "No ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const std::size_t FONTPATHNAME_LENGTH {256};
    char fontPathname[FONTPATHNAME_LENGTH] {};
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLBitmapFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());
        ftgl(ftglFont);
        setFontLoaded();
    }
    else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglBitmapFont::loadFont() - font did not load correctly: file: \"";
            std::cerr << fontPathname << "\"";
            std::cerr << std::endl;
        }
        std::exit(1);
    }
}

}
}

