
#include "mixr/graphics/fonts/ftgl/FtglBitmapFont.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/util/filesystem_utils.hpp"

#include <iostream>
#include <string>

#include <FTGL/ftgl.h>

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(FtglBitmapFont, "FTGLBitmapFonts")
EMPTY_DELETEDATA(FtglBitmapFont)
EMPTY_SLOTTABLE(FtglBitmapFont)
EMPTY_COPYDATA(FtglBitmapFont)
EMPTY_CONSTRUCTOR(FtglBitmapFont)

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

    std::string fontPathname{base::buildPath(fontDirectory(), filename())};
    const auto ftglFont = new FTGLBitmapFont(fontPathname.c_str());
    if (ftglFont != nullptr && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());
        ftgl(ftglFont);
        setFontLoaded();
    } else {
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

