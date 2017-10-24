
#include "mixr/graphics/fonts/AbstractFont.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/String.hpp"

#include "mixr/base/util/str_utils.hpp"

#include <cstring>

namespace mixr {
namespace graphics {

IMPLEMENT_PARTIAL_SUBCLASS(AbstractFont, "AbstractFont")

const double defaultFontWidth {1.0};
const double defaultFontHeight {1.0};

BEGIN_SLOTTABLE(AbstractFont)
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
END_SLOTTABLE(AbstractFont)

BEGIN_SLOT_MAP(AbstractFont)
    ON_SLOT(1, setSlotFontWidth,        base::Number)
    ON_SLOT(2, setSlotFontHeight,       base::Number)
    ON_SLOT(3, setSlotFontPosition,     base::List)
    ON_SLOT(4, setSlotBitmapWidth,      base::Number)
    ON_SLOT(5, setSlotBitmapHeight,     base::Number)
    ON_SLOT(6, setSlotFontPath,         base::String)
    ON_SLOT(7, setSlotFTGLFontFileName, base::String)
    ON_SLOT(8, setSlotLookupTable,      base::List)
    ON_SLOT(9, setSlotCharacterSpacing, base::Number);
    ON_SLOT(10, setSlotLineSpacing,     base::Number);
END_SLOT_MAP()

AbstractFont::AbstractFont()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void AbstractFont::initData()
{
   setFontWidth( defaultFontWidth );
   setFontHeight( defaultFontHeight );

   setBitmapWidth( static_cast<int>(defaultFontWidth) );
   setBitmapHeight( static_cast<int>(defaultFontHeight) );
}

AbstractFont::AbstractFont(const AbstractFont& org)
{
   STANDARD_CONSTRUCTOR()
   copyData(org, true);
}

AbstractFont::~AbstractFont()
{
   STANDARD_DESTRUCTOR()
}

AbstractFont& AbstractFont::operator=(const AbstractFont& org)
{
   if (this != &org) copyData(org, false);
   return *this;
}

AbstractFont* AbstractFont::clone() const
{
   return nullptr;
}

void AbstractFont::copyData(const AbstractFont& org, const bool cc)
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

   if (fontPath != nullptr) delete[] fontPath;
   if (fontFile != nullptr) delete[] fontFile;
   fontPath = nullptr;
   fontFile = nullptr;
   if (org.fontPath != nullptr) {
      std::size_t len {std::strlen(org.fontPath)};
      fontPath = new char[len+1];
      base::utStrcpy(fontPath,len+1,org.fontPath);
   }
   if (org.fontFile != nullptr) {
      std::size_t len {std::strlen(org.fontFile)};
      fontFile = new char[len+1];
      base::utStrcpy(fontFile,len+1,org.fontFile);
   }
   loaded = org.loaded;

   charSpacing = org.charSpacing;
   lineSpacing = org.lineSpacing;

   fWidth = org.fWidth;
   fHeight = org.fHeight;
   bWidth = org.bWidth;
   bHeight = org.bHeight;
}

void AbstractFont::deleteData()
{
   if (pLUT != nullptr) delete[] pLUT;
   pLUT = nullptr;

   if (fontPath != nullptr) delete[] fontPath;
   fontPath = nullptr;

   if (fontFile != nullptr) delete[] fontFile;
   fontFile = nullptr;
}

//------------------------------------------------------------------------------
// position() -- computes the position of the (ln, cp) pair
//------------------------------------------------------------------------------
void AbstractFont::position(const int ln, const int cp, GLdouble& px, GLdouble& py) const
{
   // Compute X position based on the one based character position (column)
   px = leftSide + charSpacing*(cp-1);

   // Compute Y position based on the one based line number (row)
   py = topSide - lineSpacing*ln;
}

//------------------------------------------------------------------------------
// setTextOrigin() -- sets the upper left corner
//------------------------------------------------------------------------------
void AbstractFont::setTextOrigin(const GLdouble x, const GLdouble y)
{
    leftSide = x;
    topSide = y;
}

//------------------------------------------------------------------------------
// xferChars() -- transfer characters
//------------------------------------------------------------------------------
int AbstractFont::xferChars(char* const outp, const std::size_t BUF_SIZE, const char* const inp, const unsigned int n) const
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
      base::utStrncpy(outp,BUF_SIZE,inp,n);
   }
   outp[n] = '\0';
   return static_cast<int>(std::strlen(outp));
}

//------------------------------------------------------------------------------
// setSlotFontWidth () -- sets the font width
//------------------------------------------------------------------------------
bool AbstractFont::setSlotFontWidth(const base::Number* const sfwobj)
{
    if (sfwobj != nullptr) setFontWidth( sfwobj->getDouble() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontHeight() - sets the font height
//------------------------------------------------------------------------------
bool AbstractFont::setSlotFontHeight (const base::Number* const sfhobj)
{
    if (sfhobj != nullptr) setFontHeight( sfhobj->getDouble() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontPosition() - sets the font position
//------------------------------------------------------------------------------
bool AbstractFont::setSlotFontPosition (const base::List* const sfpobj)
{
    bool ok = true;
    if (sfpobj != nullptr) {
        double values[2];
        int n = sfpobj->getNumberList(values, 2);
        ok = (n == 2);
        if (ok) setTextOrigin(values[0],values[1]);
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
bool AbstractFont::setSlotBitmapWidth(const base::Number* const sbwobj)
{
    if (sbwobj != nullptr) setBitmapWidth( sbwobj->getInt() );
    return true;
}

//------------------------------------------------------------------------------
//  setBitmapHeight() - sets the bitmap height
//------------------------------------------------------------------------------
bool AbstractFont::setSlotBitmapHeight(const base::Number* const sbhobj)
{
    if (sbhobj != nullptr) setBitmapHeight( sbhobj->getInt() );
    return true;
}

//------------------------------------------------------------------------------
//  setSlotFontPath() - sets the path to the font directory
//------------------------------------------------------------------------------
bool AbstractFont::setSlotFontPath(const base::String* const str)
{
    bool ok = true;
    if (str != nullptr) {
        std::size_t j {str->len()};
        if (j > 0) {
            fontPath = new char[j+1];
            utStrcpy(fontPath, j+1, *str);
        }
        else {
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
bool AbstractFont::setSlotFTGLFontFileName(const base::String* const str)
{
    bool ok = true;
    if (str != nullptr) {
        std::size_t j = str->len();
        if (j > 0) {
            fontFile = new char[j+1];
            utStrcpy(fontFile, j+1, *str);
        }
        else {
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
bool AbstractFont::setSlotLookupTable(const base::List* const sltobj)
{
    bool ok = true;
    if (sltobj != nullptr) {
        // Load the LUT
        if (pLUT == nullptr) pLUT = new unsigned char[LUT_SIZE];
        int values[LUT_SIZE];
        int nn = sltobj->getNumberList(values, LUT_SIZE);
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
    }
    else {
          if (isMessageEnabled(MSG_ERROR)) {
             std::cerr << "Font::setLookupTable: Nothing in the lookup table." << std::endl;
          }
        ok = false;
    }
    return ok;
}

bool AbstractFont::setSlotCharacterSpacing(const base::Number* const newCharSpacing)
{
    // set our character spacing
    if (newCharSpacing != nullptr) setCharacterSpacing( newCharSpacing->getFloat() );
    return true;
}

bool AbstractFont::setSlotLineSpacing(const base::Number* const newLineSpacing)
{
    // set our line spacing
    if (newLineSpacing != nullptr) setLineSpacing( newLineSpacing->getFloat() );
    return true;
}

}
}
