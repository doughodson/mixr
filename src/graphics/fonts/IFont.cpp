
#include "mixr/graphics/fonts/IFont.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/util/str_utils.hpp"

#include <cstring>
#include <string>

namespace mixr {
namespace graphics {

IMPLEMENT_PARTIAL_SUBCLASS(IFont, "IFont")

const double defaultFontWidth{1.0};
const double defaultFontHeight{1.0};

BEGIN_SLOTTABLE(IFont)
        "fontWidth",
        "fontHeight",
        "fontPosition",         // Upper/Left origin of the text field
        "bitmapWidth",
        "bitmapHeight",
        "path",                 // Path to the font directory
        "file",                 // FTGL Font file name - will be altered in each instance of FTGLFont
        "lut",                  // Lookup Table
        "characterSpacing",     // spacing for each character
        "lineSpacing",          // spacing for each line
END_SLOTTABLE(IFont)

BEGIN_SLOT_MAP(IFont)
    ON_SLOT(1, setSlotFontWidth,        base::INumber)
    ON_SLOT(2, setSlotFontHeight,       base::INumber)
    ON_SLOT(3, setSlotFontPosition,     base::List)
    ON_SLOT(4, setSlotBitmapWidth,      base::Integer)
    ON_SLOT(5, setSlotBitmapHeight,     base::Integer)
    ON_SLOT(6, setSlotFontPath,         base::String)
    ON_SLOT(7, setSlotFTGLFontFileName, base::String)
    ON_SLOT(8, setSlotLookupTable,      base::List)
    ON_SLOT(9, setSlotCharacterSpacing, base::INumber);
    ON_SLOT(10, setSlotLineSpacing,     base::INumber);
END_SLOT_MAP()

IFont::IFont()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void IFont::initData()
{
   setFontWidth( defaultFontWidth );
   setFontHeight( defaultFontHeight );

   setBitmapWidth( static_cast<int>(defaultFontWidth) );
   setBitmapHeight( static_cast<int>(defaultFontHeight) );
}

IFont::IFont(const IFont& org)
{
   STANDARD_CONSTRUCTOR()
   copyData(org, true);
}

IFont::~IFont()
{
   STANDARD_DESTRUCTOR()
}

IFont& IFont::operator=(const IFont& org)
{
   if (this != &org) copyData(org, false);
   return *this;
}

IFont* IFont::clone() const
{
   return nullptr;
}

void IFont::copyData(const IFont& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   leftSide = org.leftSide;
   topSide = org.topSide;
   b = org.b;

   if (org.pLUT != nullptr) {
      if (pLUT == nullptr) {
         pLUT = new unsigned char[LUT_SIZE];
      }
      for (unsigned int i = 0; i < LUT_SIZE; i++) {
         pLUT[i] = org.pLUT[i];
      }
   }
   else {
      if (pLUT != nullptr) delete[] pLUT;
      pLUT = nullptr;
   }

   pFTGL = org.pFTGL;

   fontPath = org.fontPath;
   fontFile = org.fontFile;

   loaded = org.loaded;

   charSpacing = org.charSpacing;
   lineSpacing = org.lineSpacing;

   fWidth = org.fWidth;
   fHeight = org.fHeight;
   bWidth = org.bWidth;
   bHeight = org.bHeight;
}

void IFont::deleteData()
{
   if (pLUT != nullptr) delete[] pLUT;
   pLUT = nullptr;
}

//------------------------------------------------------------------------------
// position() -- computes the position of the (ln, cp) pair
//------------------------------------------------------------------------------
void IFont::position(const int ln, const int cp, GLdouble& px, GLdouble& py) const
{
   // Compute X position based on the one based character position (column)
   px = leftSide + charSpacing*(cp-1);

   // Compute Y position based on the one based line number (row)
   py = topSide - lineSpacing*ln;
}

//------------------------------------------------------------------------------
// setTextOrigin() -- sets the upper left corner
//------------------------------------------------------------------------------
void IFont::setTextOrigin(const GLdouble x, const GLdouble y)
{
    leftSide = x;
    topSide = y;
}

//------------------------------------------------------------------------------
// xferChars() -- transfer characters
//------------------------------------------------------------------------------
int IFont::xferChars(char* const outp, const std::size_t BUF_SIZE, const char* const inp, const unsigned int n) const
{
    if (outp == nullptr || inp == nullptr) return 0;
    if (n >= BUF_SIZE) return 0;

   // Buffer the string to translate the characters with the LUT,
   // as required, and to ensure that the string is terminated.
   if (pLUT != nullptr) {
      for (unsigned int i = 0; i < n; i++) {
          unsigned char idx = static_cast<unsigned char>(inp[i]);
          outp[i] = static_cast<char>(pLUT[idx]);
      }
   }
   else {
      base::utStrncpy(outp, BUF_SIZE, inp, n);
   }
   outp[n] = '\0';
   return static_cast<int>(std::strlen(outp));
}

//------------------------------------------------------------------------------
// setSlotFontWidth () -- sets the font width
//------------------------------------------------------------------------------
bool IFont::setSlotFontWidth(const base::INumber* const x)
{
    if (x != nullptr) setFontWidth( x->asDouble() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontHeight() - sets the font height
//------------------------------------------------------------------------------
bool IFont::setSlotFontHeight (const base::INumber* const x)
{
    if (x != nullptr) setFontHeight( x->asDouble() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontPosition() - sets the font position
//------------------------------------------------------------------------------
bool IFont::setSlotFontPosition (const base::List* const x)
{
    bool ok{true};
    if (x != nullptr) {
        double values[2]{};
        std::size_t n{x->getNumberList(values, 2)};
        ok = (n == 2);
        if (ok) setTextOrigin(values[0], values[1]);
        else {
            if (isMessageEnabled(MSG_ERROR)) {
                std::cerr << "Font::setFontPosition: Values are invalid or missing" << std::endl;
            }
            ok = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotBitmapWidth() - sets the bitmap width
//------------------------------------------------------------------------------
bool IFont::setSlotBitmapWidth(const base::Integer* const x)
{
    if (x != nullptr) setBitmapWidth( x->asInt() );
    return true;
}

//------------------------------------------------------------------------------
//  setBitmapHeight() - sets the bitmap height
//------------------------------------------------------------------------------
bool IFont::setSlotBitmapHeight(const base::Integer* const x)
{
    if (x != nullptr) setBitmapHeight( x->asInt() );
    return true;
}

// set font path
bool IFont::setSlotFontPath(const base::String* const x)
{
    bool ok{true};
    if (x != nullptr) {
        std::size_t j{x->len()};
        if (j > 0) {
            fontPath = x->c_str();
//            fontPath = new char[j+1];
//            base::utStrcpy(fontPath, j+1, (*str).c_str());
        } else {
              if (isMessageEnabled(MSG_ERROR)) {
                  std::cerr << "Font::setFontPath: Invalid font path value" << std::endl;
              }
            ok = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotFTGLFontFileName() - sets the FTGL Font File Name
//------------------------------------------------------------------------------
bool IFont::setSlotFTGLFontFileName(const base::String* const x)
{
    bool ok{true};
    if (x != nullptr) {
        std::size_t j = x->len();
        if (j > 0) {
            fontFile = x->c_str();
//            fontFile = new char[j+1];
//            base::utStrcpy(fontFile, j+1, (*str).c_str());
        } else {
           if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Font::setFTGLFontFileName: File name invalid or missing" << std::endl;
           }
            ok = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotLookupTable() - sets the lookup table
//------------------------------------------------------------------------------
bool IFont::setSlotLookupTable(const base::List* const x)
{
    // Load the LUT
    if (pLUT == nullptr) pLUT = new unsigned char[LUT_SIZE];
    int values[LUT_SIZE];
    int nn = x->getNumberList(values, LUT_SIZE);
    int ii = 0;
    // Transfer the table
    while (ii < nn) {
        pLUT[ii] = ( (values[ii] >= 0 && values[ii] <= 255) ? char(values[ii]) : char(ii) );
        ii++;
    }
    // Pad the end of the table
    while (ii < LUT_SIZE) {
        pLUT[ii] = char(ii);
        ii++;
    }
    return true;
}

bool IFont::setSlotCharacterSpacing(const base::INumber* const x)
{
    // set our character spacing
    setCharacterSpacing(x->asDouble());
    return true;
}

bool IFont::setSlotLineSpacing(const base::INumber* const x)
{
    // set our line spacing
    setLineSpacing(x->asDouble());
    return true;
}

}
}
