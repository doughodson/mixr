
#ifndef __mixr_graphics_BmpTexture_H__
#define __mixr_graphics_BmpTexture_H__

#include "mixr/graphics/Texture.hpp"
#include <GL/gl.h>

namespace mixr {
    namespace base { class String; }

namespace graphics {

//------------------------------------------------------------------------------
// Class: BmpTexture
//
// Description: Read/Write a BMP file (use loadTexture() to read)
//
// Factory name:
// Slots:
//    path    <String>     ! Path to the texture directory (default: 0)
//    file    <String>     ! BmpTexture file name (default: 0)
//
//------------------------------------------------------------------------------
class BmpTexture : public Texture
{
   DECLARE_SUBCLASS(BmpTexture, Texture)

public:
   BmpTexture();

   // The filename of the current texture.
   const char* getTextureFileName() const;
   bool setTextureFileName(const char* const fileName);

   // The path to the current textures directory.
   const char* getTexturePath() const;
   bool setTexturePath(const char* const path);

   // Read the BMP file
   bool readFile();

   // Write the BMP file
   bool writeFile();

   void loadTexture() override;

private:
   char texPath[MAX_PATH_LEN+1] {};       // Path to texture directory
   char texFile[MAX_FILENAME_LEN+1] {};   // BmpTexture filename

private:
   // slot table helper methods
   bool setSlotTexturePath(const base::String* const);
   bool setSlotTextureFileName(const base::String* const);
};

//------------------------------------------------------------------------------
// Inline routines
//------------------------------------------------------------------------------
inline const char* BmpTexture::getTexturePath() const       { return texPath; }
inline const char* BmpTexture::getTextureFileName() const   { return texFile; }

}
}

#endif
