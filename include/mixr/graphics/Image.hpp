
#ifndef __mixr_graphics_Image_HPP__
#define __mixr_graphics_Image_HPP__

#include "mixr/base/IObject.hpp"

#include "mixr/base/util/platform_api.hpp"
#include <GL/gl.h>

namespace mixr {
   namespace base { class Number; }

namespace graphics {
   struct BITMAPINFOHEADER_X;

//------------------------------------------------------------------------------
// Class: Image
// Description: Base class for images and textures
//------------------------------------------------------------------------------
class Image : public base::IObject
{
   DECLARE_SUBCLASS(Image, base::IObject)

public:
   // Max length of file name or directory path
   static const unsigned int MAX_PATH_LEN = 256;
   static const unsigned int MAX_FILENAME_LEN = 64;

   // pixel size in bytes { RGB }
   static const int PIXEL_SIZE = 3;

public:
   Image();

   Image(
      const unsigned int w,   // Width of the image
      const unsigned int h,   // Height of the image
      const unsigned int nc,  // Number of components
      const GLenum format,    // format: GL_RGB, GL_BGRA_EXT, etc
      const GLubyte* const image // pixels to copy
   );

   unsigned int getWidth() const;         // width in pixels
   unsigned int getHeight() const;        // height in pixels
   unsigned int getNumComponents() const; // Number of color components (3 or 4)
   GLenum getFormat() const;              // format: GL_RGB, GL_BGRA_EXT, etc

   const GLubyte* getPixels() const;      // Pointer to our image pixels

   // Resolution (pixels per meter)
   unsigned int getXResolutionPPM() const;
   unsigned int getYResolutionPPM() const;

   // set functions
   virtual bool setWidth(const unsigned int w);
   virtual bool setHeight(const unsigned int h);
   virtual bool setFormat(const GLenum f);
   virtual bool setNumComponents(const unsigned int n);
   virtual bool setXResolutionPPM(const unsigned int ppm);
   virtual bool setYResolutionPPM(const unsigned int ppm);

   // Sets our image to a copy of this image
   virtual bool setPixels(const GLubyte* const image);

   // Read/write a bitmap (BMP) format file
   virtual bool readFileBMP(const char* const filename, const char* const path=nullptr);
   virtual bool writeFileBMP(const char* const filename, const char* const path=nullptr);

private:
   GLubyte* readRgbValuesBMP(FILE* const fp, const unsigned int offset, const BITMAPINFOHEADER_X* const bmfi) const;
   GLubyte* readColorValuesBMP(FILE* const fp, const unsigned int offset, const BITMAPINFOHEADER_X* const bmfi) const;

   unsigned int width {};           // width in pixels
   unsigned int height {};          // height in pixels
   unsigned int numComponents {};   // Number of color components (3 or 4)
   GLenum format {GL_RGB};          // GL_RGB, GL_BGRA_EXT, etc.

   GLubyte* pixels {};              // Pointer to our pixels; number of pixel bytes must
                                    // be equal to (width*height*numComponents)

   unsigned int xPixPerMeter {3937};   // X pixels per meter (about 100 pixels per inch)
   unsigned int yPixPerMeter {3937};   // Y pixels per meter (about 100 pixels per inch)
};

inline unsigned int Image::getWidth() const           { return width; }
inline unsigned int Image::getHeight() const          { return height; }
inline unsigned int Image::getNumComponents() const   { return numComponents; }
inline GLenum Image::getFormat() const                { return format; }
inline const GLubyte* Image::getPixels() const        { return pixels; }

inline unsigned int Image::getXResolutionPPM() const  { return xPixPerMeter; }
inline unsigned int Image::getYResolutionPPM() const  { return yPixPerMeter; }

}
}

#endif
