
#include "mixr/graphics/BmpTexture.hpp"
#include "mixr/graphics/Display.hpp"
#include "mixr/base/Identifier.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/util/str_utils.hpp"

#include <cstdio>

#if defined(__MINGW32__)
   #include <GL/glext.h>
#endif

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(BmpTexture, "BitmapTexture")
EMPTY_DELETEDATA(BmpTexture)

BEGIN_SLOTTABLE(BmpTexture)
    "path",         // 1) Path to the texture directory
    "file",         // 2) BmpTexture file name
END_SLOTTABLE(BmpTexture)

BEGIN_SLOT_MAP(BmpTexture)
    ON_SLOT(1, setSlotTexturePath,     base::String)
    ON_SLOT(2, setSlotTextureFileName, base::String)
END_SLOT_MAP()

BmpTexture::BmpTexture()
{
   STANDARD_CONSTRUCTOR()
}

void BmpTexture::copyData(const BmpTexture& org, const bool)
{
   BaseClass::copyData(org);

   base::utStrcpy(texPath, sizeof(texPath), org.texPath);
   base::utStrcpy(texFile, sizeof(texFile), org.texFile);
}

//------------------------------------------------------------------------------
// Set routines
//------------------------------------------------------------------------------
bool BmpTexture::setTextureFileName(const char* const fileName)
{
   if (fileName != nullptr) {
      base::utStrcpy(texFile, sizeof(texFile), fileName);
   }
   else {
      texFile[0] = '\0';
   }
   return true;
}

bool BmpTexture::setTexturePath(const char* const path)
{
   if (path != nullptr) {
      base::utStrcpy(texPath, sizeof(texPath), path);
   }
   else {
      texPath[0] = '\0';
   }
   return true;
}

//------------------------------------------------------------------------------
// loadTexture() -- load the texture
//------------------------------------------------------------------------------
void BmpTexture::loadTexture()
{
   // If needed, read the BMP file
   if (getPixels() == nullptr) readFile();

   // Load the texture
   if (getPixels() != nullptr) BaseClass::loadTexture();
}

//------------------------------------------------------------------------------
// Read the BMP file
//------------------------------------------------------------------------------
bool BmpTexture::readFile()
{
   return readFileBMP(getTextureFileName(), getTexturePath());
}

//------------------------------------------------------------------------------
// Write the BMP file
//------------------------------------------------------------------------------
bool BmpTexture::writeFile()
{
   return writeFileBMP(getTextureFileName(), getTexturePath());
    }

//------------------------------------------------------------------------------
// SLOT functions
//------------------------------------------------------------------------------

// setSlotTexturePath() -- sets the path to the textures directory
bool BmpTexture::setSlotTexturePath(const base::String* const s)
{
   return setTexturePath((*s).c_str());
}

// setSlotTextureFileName() -- sets the texture file name
bool BmpTexture::setSlotTextureFileName(const base::String* const s)
{
   return setTextureFileName((*s).c_str());
}

}
}
