
#include "mixr/graphics/fonts/ftgl/FtglHaloFont.hpp"

#include "mixr/graphics/Graphic.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/util/filesystem_utils.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/colors/IColor.hpp"

#include <iostream>
#include <string>

#include <FTGL/ftgl.h>

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(FtglHaloFont, "FTGLHaloFont")

BEGIN_SLOTTABLE(FtglHaloFont)
    "halocolor",
    "linewidth",
END_SLOTTABLE(FtglHaloFont)

BEGIN_SLOT_MAP(FtglHaloFont)
    ON_SLOT(1, setSlotHaloColor, base::IColor)
    ON_SLOT(2, setSlotLinewidth, base::Number)
END_SLOT_MAP()

FtglHaloFont::FtglHaloFont()
{
    STANDARD_CONSTRUCTOR();
}

void FtglHaloFont::copyData(const FtglHaloFont& org, const bool)
{
    BaseClass::copyData(org);
    if (haloColor != nullptr) haloColor->unref();
    haloColor = nullptr;
    linewidth = org.linewidth;
}

void FtglHaloFont::deleteData()
{
    if (haloColor != nullptr) haloColor->unref();
    haloColor = nullptr;
    if (outline != nullptr) delete outline;
    outline = nullptr;
}

//------------------------------------------------------------------------------
// Halo Font functions
// FtglHaloFont::outputText() -- FtglHaloFont output routines
//------------------------------------------------------------------------------
void FtglHaloFont::outputText(const double x, const double y, const char* txt, const int n, const bool vf, const bool rf)
{
    GLfloat ocolor[4]{};
    glGetFloatv(GL_CURRENT_COLOR, ocolor);

    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }

    // Prepare the output text
    char cbuf[MSG_BUF_LEN]{};
    int nn {xferChars(cbuf,MSG_BUF_LEN,txt,n)};
    if (nn <= 0) return;

    if (haloColor != nullptr) Graphic::lcColor3(haloColor->red(), haloColor->green(), haloColor->blue());
    // default to black if we have no color specified
    else glColor3f(0,0,0);

    // now render the outline font over it!
    if (!rf) {
        if (outline != nullptr) {
            glPushMatrix();
            glTranslated(x, y, -0.01f);
            // get our current linewidth
            GLfloat lw {};
            glGetFloatv(GL_LINE_WIDTH, &lw);
            glLineWidth(linewidth);
            glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
            if (vf) {
                // Vertical text
                GLdouble dy {getLineSpacing()};
                if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
                char cc[2] {};
                cc[1] = '\0';
                for (int i = 0; i < nn; i++) {
                    cc[0] = cbuf[i];
                    outline->Render(cc);
                    glTranslated(0.0, -dy, 0.0);
                }
            } else {
                // Normal text
                outline->Render(cbuf);
            }
            glLineWidth(lw);
            glPopMatrix();
        } else {
              if (isMessageEnabled(MSG_ERROR)) {
                  std::cerr << "FtglHaloFont::outputText() - no outline font available" << std::endl;
              }
        }
    }

    glColor4fv(ocolor);

    // output the text
    const auto ftgl1 = static_cast<FTGLPolygonFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glTranslated(x, y, 0.0);
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy{getLineSpacing()};
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2]{};
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
            std::cerr << "FtglPolygonFont::outputText() - no Polygon font available" << std::endl;
        }
    }
}

void FtglHaloFont::outputText(const char* txt, const int n, const bool vf, const bool rf)
{
    GLfloat ocolor[4]{};
    glGetFloatv(GL_CURRENT_COLOR, ocolor);

    // Make sure we have characters to print
    if (n <= 0) return;

    // Make sure we have a loaded font
    if (isNotLoaded()) {
        loadFont();
        if (isNotLoaded()) throw new ExpInvalidFont();
    }
    // Prepare the output text
    char cbuf[MSG_BUF_LEN]{};
    int nn{xferChars(cbuf,MSG_BUF_LEN,txt,n)};
    if (nn <= 0) return;

    if (haloColor != nullptr) Graphic::lcColor3(haloColor->red(), haloColor->green(), haloColor->blue());
    // default to black if we have no color specified
    else glColor3f(0,0,0);

    if (!rf) {
        // Output the outline in black
        if (outline != nullptr) {
            glPushMatrix();
            // get our current linewidth
            GLfloat lw{};
            glGetFloatv(GL_LINE_WIDTH, &lw);
            glLineWidth(linewidth);
            glTranslatef(0,0,-0.01f);
            glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
            if (vf) {
                // Vertical text
                GLdouble dy{getLineSpacing()};
                if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
                char cc[2]{};
                cc[1] = '\0';
                for (int i = 0; i < nn; i++) {
                    cc[0] = cbuf[i];
                    outline->Render(cc);
                    glTranslated(0.0, -dy, 0.0);
                }
            } else {
                outline->Render(cbuf);
            }
            glLineWidth(lw);
            glPopMatrix();
        } else {
            if (isMessageEnabled(MSG_ERROR)) {
                std::cerr << "FtglHaloFont::outputText() - no outline font available" << std::endl;
            }
        }
    }

    glColor4fv(ocolor);

    // output the text
    const auto ftgl1 = static_cast<FTGLPolygonFont*>(FTGL());
    if (ftgl1 != nullptr) {
        glPushMatrix();
        glScalef(static_cast<GLfloat>(getFontWidth()), static_cast<GLfloat>(getFontHeight()), 1.0f);
        if (vf) {
            // Vertical text
            GLdouble dy{getLineSpacing()};
            if (getFontHeight() != 0.0) dy = getLineSpacing() / getFontHeight();
            char cc[2]{};
            cc[1] = '\0';
            for (int i = 0; i < nn; i++) {
                cc[0] = cbuf[i];
                ftgl1->Render(cc);
                glTranslated(0.0, -dy, 0.0);
            }
        } else {
            ftgl1->Render(cbuf);
        }
        glPopMatrix();
    } else {
          if (isMessageEnabled(MSG_ERROR)) {
              std::cerr << "FtglHaloFont::outputText() - no Polygon font available" << std::endl;
          }
    }
}

void FtglHaloFont::loadFont()
{
    if (isLoaded()) return;

    std::string fontPathname{base::buildPath(fontDirectory(), filename())};
    const auto ftglFont = new FTGLPolygonFont(fontPathname.c_str());
    if (ftglFont != nullptr && !ftglFont->Error()) {
        // set the face size and return the pointer, then tell our base class that we have a loaded font
        ftglFont->FaceSize(getFaceSize());
        ftgl(ftglFont);
        setFontLoaded();
    } else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglHaloFont::loadFont() - font did not load correctly: file: \"";
            std::cerr << fontPathname << "\"";
            std::cerr << std::endl;
        }
        std::exit(1);
    }

    // now create the outline font over it
    outline = new FTGLOutlineFont(fontPathname.c_str());
    if (outline != nullptr && !outline->Error()) {
        // set the face size
        outline->FaceSize(getFaceSize());
    } else {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "FtglHaloFont::loadFont() - font did not load correctly: file: \"";
            std::cerr << fontPathname << "\"";
            std::cerr << std::endl;
        }
        std::exit(1);
    }

}

bool FtglHaloFont::setSlotHaloColor(base::IColor* x)
{
    // set our halo color
    if (haloColor != nullptr) haloColor->unref();
    haloColor = nullptr;
    if (x != nullptr) {
        haloColor = x;
        haloColor->ref();
    }
    return true;
}

bool FtglHaloFont::setSlotLinewidth(const base::Number* const x)
{
    linewidth = static_cast<float>(x->asDouble());
    return true;
}

}
}

