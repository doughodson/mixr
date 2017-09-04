
#include "mixr/graphics/fonts/FtglExtrdFont.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/util/str_utils.hpp"

#include <iostream>

#include <FTGL/ftgl.h>

namespace mixr {
namespace graphics {

const float FtglExtrdFont::DEFAULT_DEPTH {5.0};

IMPLEMENT_SUBCLASS(FtglExtrdFont, "FTGLExtrdFonts")
EMPTY_DELETEDATA(FtglExtrdFont)

BEGIN_SLOTTABLE(FtglExtrdFont)
    "depth",
END_SLOTTABLE(FtglExtrdFont)

BEGIN_SLOT_MAP(FtglExtrdFont)
    ON_SLOT(1, setSlotDepth, base::Number)
END_SLOT_MAP()

FtglExtrdFont::FtglExtrdFont()
{
    STANDARD_CONSTRUCTOR();
}

void FtglExtrdFont::copyData(const FtglExtrdFont& org, const bool)
{
    BaseClass::copyData(org);
    depth = org.depth;
}

//------------------------------------------------------------------------------
// FtglExtrdFont slot table methods
//------------------------------------------------------------------------------
bool FtglExtrdFont::setSlotDepth(const base::Number* const newDepth)
{
    // set our depth
    if (newDepth != nullptr) depth = newDepth->getFloat();
    else depth = DEFAULT_DEPTH;
    return true;
}

//------------------------------------------------------------------------------
// Extruded font functions
// FtglExtrdFont::outputText() -- FTGL output routines
//------------------------------------------------------------------------------
void FtglExtrdFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool)
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

    // Output the text
    const auto ftgl1 = static_cast<FTGLExtrdFont*>(FTGL());
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
            std::cerr << "FtglExtrdFont::outputText() - no extrude font available" << std::endl;
        }
    }
}

void FtglExtrdFont::outputText(const char* txt, const int n, const bool vf, const bool)
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

    // Output the text
    const auto ftgl1 = static_cast<FTGLExtrdFont*>(FTGL());
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
            std::cerr << "FTGLExtrdFont::outputText() - no extruded font available" << std::endl;
        }
    }
}

// Load FtglExtrdFont
void FtglExtrdFont::loadFont()
{
    if (isLoaded()) return;

    // Check for required parameters
    if ( filename() == nullptr ) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FTGLExtrdFont::loadFont() - no ttf file" << std::endl;
        }
        return;
    }

    // Generate filename
    const std::size_t FONTPATHNAME_LENGTH {256};
    char fontPathname[FONTPATHNAME_LENGTH] {};
    if (fontDirectory() != nullptr) base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, fontDirectory());
    else base::utStrcpy(fontPathname, FONTPATHNAME_LENGTH, "./");
    base::utStrcat(fontPathname, FONTPATHNAME_LENGTH, filename());

    const auto ftglFont = new FTGLExtrdFont(fontPathname);
    if (ftglFont != nullptr && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());
        ftgl(ftglFont);
        // we'll put this here for now
        ftglFont->Depth(10);
        setFontLoaded();
    } else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglExtrdFont::loadFont() - font did not load correctly: file: \"";
            std::cerr << fontPathname << "\"";
            std::cerr << std::endl;
        }
        std::exit(1);
    }
}

}
}

