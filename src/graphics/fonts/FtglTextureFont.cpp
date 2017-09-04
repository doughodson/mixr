
#include "mixr/graphics/fonts/FtglTextureFont.hpp"

#include "mixr/base/util/str_utils.hpp"

#include <iostream>

#include <FTGL/ftgl.h>

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(FtglTextureFont, "FTGLTextureFont")
EMPTY_SLOTTABLE(FtglTextureFont)
EMPTY_DELETEDATA(FtglTextureFont)

FtglTextureFont::FtglTextureFont()
{
    STANDARD_CONSTRUCTOR();
}

void FtglTextureFont::copyData(const FtglTextureFont& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// Texture Font functions
// FtglTextureFont::outputText() -- FTGLTextureFont output routines
//------------------------------------------------------------------------------
void FtglTextureFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
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
    const auto ftgl1 = static_cast<FTGLTextureFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy {getLineSpacing()};
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2] {};
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);
                glTranslated(0.0, -dy, 0.0);
            }
        } else {
            // Normal text
            ftgl1->Render(cbuf);
        }
        glPopMatrix();
    } else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglTextureFont::outputText() - no Texture font available" << std::endl;
          }
    }
}

void FtglTextureFont::outputText(const char* txt, const int n, const bool vf, const bool)
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
    const auto ftgl1 = static_cast<FTGLTextureFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy {getLineSpacing()};
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2] {};
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);
                glTranslated(0.0, -dy, 0.0);
            }
        } else {
            // Normal text
            ftgl1->Render(cbuf);
        }
        glPopMatrix();
    } else {
        if (isMessageEnabled(MSG_ERROR)) {
             std::cerr << "FtglTextureFont::outputText() - no Texture font available" << std::endl;
        }
    }
}

void FtglTextureFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters

    if ( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglTextureFont::loadFont() - no ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const std::size_t FONTPATHNAME_LENGTH {256};
    char fontPathname[FONTPATHNAME_LENGTH] {};
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLTextureFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());
        ftgl(ftglFont);
        setFontLoaded();
    } else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglTextureFont::loadFont() - font did not load correctly: file: \"";
            std::cerr << fontPathname << "\"";
            std::cerr << std::endl;
        }
        std::exit(1);
    }
}

}
}
