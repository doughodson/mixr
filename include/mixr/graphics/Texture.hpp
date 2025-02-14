
#ifndef __mixr_graphics_Texture_HPP__
#define __mixr_graphics_Texture_HPP__

#include "mixr/graphics/Image.hpp"

namespace mixr {
namespace base { class Identifier; class Integer; class INumber; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: Texture
//
// Description: Top level texture class for general (simple) 2D textures.
//
// NOTE: Currently reads only simple, uncompressed Microsoft Bitmap (BMP) files.
//
// NOTE: The width & height of the bitmap is truncated to powers of 2.
// Factory name: Texture
//
// Slots:
//    width       <Integer>      ! Size of the texture map (default: 256)
//    height      <Integer>      ! Size of the texture map.(default: 256)
//    redScale    <INumber>      ! for glPixelTransferf(GL_RED_SCALE) (default: 1.0)
//    redBias     <INumber>      ! for glPixelTransferf(GL_RED_BIAS) (default: 0.0)
//    greenScale  <INumber>      ! for glPixelTransferf(GL_GREEN_SCALE) (default: 1.0)
//    greenBias   <INumber>      ! for glPixelTransferf(GL_GREEN_BIAS) (default: 0.0)
//    blueScale   <INumber>      ! for glPixelTransferf(GL_BLUE_SCALE) (default: 1.0)
//    blueBias    <INumber>      ! for glPixelTransferf(GL_BLUE_BIAS) (default: 0.0)
//    wrapS       <Identifier>   ! for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S) (default: GL_REPEAT)
//    wrapT       <Identifier>   ! for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T) (default: GL_REPEAT)
//    magFilter   <Identifier>   ! for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER) (default: GL_NEAREST)
//    minFilter   <Identifier>   ! for glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER) (default: GL_NEAREST)
//
//
// Public member functions:
//
//   GLunit getTexture()
//      Returns the texture.
//
// glTexParameteri() Attributes
//   int getWrapS()
//   int getWrapT()
//      Returns the wrapS/wrapT value.
//
//   int getMagFilter()
//      Returns the magFilter value.
//
//   int getMinFilter()
//      Returns the minFilter value.
//
//   bool setWrapS(int v)
//      Sets the wrapS value to v and returns true if successful.
//
//   bool setWrapT(int v)
//      Sets the wrapT value to v and returns true if successful.
//
//   bool setMagFilter(int v)
//      Sets the magFilter value to v and returns true if successful.
//
//   bool setMinFilter(int v)
//      Sets the minFilter value to v and returns true if successful.
//
// glPixelTransferf() Attributes
//   double getRedScale()
//      Returns the redScale value.
//
//   double getRedBias()
//      Returns the redBias value.
//
//   double getGreenScale()
//      Returns the greenScale value.
//
//   double getGreenBias()
//      Returns the greenBias value.
//
//   double getBlueScale()
//      Returns the blueScale value.
//
//   double getBlueBias()
//      Returns the blueBias value.
//
//   bool setRedScale(double s)
//      Sets the redScale value to s and returns true if successful.
//
//   bool setRedBias(double b)
//      Sets the redBias value to b and returns true if successful.
//
//   bool setGreenScale(double s)
//      Sets the greenScale value to s and returns true if successful.
//
//   bool setGreenBias(double b)
//      Sets the greenBias value to b and returns true if successful.
//
//   bool setBlueScale(double s)
//      Sets the blueScale value to s and returns true if successful.
//
//   bool setBlueBias(double b)
//      Sets the blueBias value to b and returns true if successful.
//
//------------------------------------------------------------------------------
class Texture : public Image
{
   DECLARE_SUBCLASS(Texture, Image)

public:
   Texture();

   // texture id
   GLuint getTexture() const;
   virtual void setTexture(const GLuint t);

   // glTexParameteri() Attributes
   int getWrapS() const;
   int getWrapT() const;
   int getMagFilter() const;
   int getMinFilter() const;
   virtual bool setWrapS(const int v);
   virtual bool setWrapT(const int v);
   virtual bool setMagFilter(const int v);
   virtual bool setMinFilter(const int v);

   // glPixelTransferf() Attributes
   GLfloat getRedScale() const;
   GLfloat getRedBias() const;
   GLfloat getGreenScale() const;
   GLfloat getGreenBias() const;
   GLfloat getBlueScale() const;
   GLfloat getBlueBias() const;
   virtual bool setRedScale(const GLfloat);
   virtual bool setRedBias(const GLfloat);
   virtual bool setGreenScale(const GLfloat);
   virtual bool setGreenBias(const GLfloat);
   virtual bool setBlueScale(const GLfloat);
   virtual bool setBlueBias(const GLfloat);

   virtual void loadTexture();

   bool setWidth(const unsigned int w) override;
   bool setHeight(const unsigned int h) override;

private:
   void initData();

   GLuint texture {};             // open GL texture identifier

   // glTexParameteri() Attributes
   int wrapS {GL_REPEAT};         // GL_REPEAT (default) or GL_CLAMP
   int wrapT {GL_REPEAT};         // GL_REPEAT (default) or GL_CLAMP
   int magFilter {GL_NEAREST};    // GL_NEAREST (default) or GL_LINEAR
   int minFilter {GL_NEAREST};    // GL_NEAREST (default) or GL_LINEAR

   // glPixelTransferf() Attributes
   GLfloat redScale {1.0};
   GLfloat redBias {0.0};
   GLfloat greenScale {1.0};
   GLfloat greenBias {0.0};
   GLfloat blueScale {1.0};
   GLfloat blueBias {0.0};

private:
   // slot table helper methods
   bool setSlotWidth(const base::Integer* const);
   bool setSlotHeight(const base::Integer* const);
   bool setSlotRedScale(const base::INumber* const);
   bool setSlotRedBias(const base::INumber* const);
   bool setSlotGreenScale(const base::INumber* const);
   bool setSlotGreenBias(const base::INumber* const);
   bool setSlotBlueScale(const base::INumber* const);
   bool setSlotBlueBias(const base::INumber* const);
   bool setSlotWrapS(const base::Identifier* const);
   bool setSlotWrapT(const base::Identifier* const);
   bool setSlotMagFilter(const base::Identifier* const);
   bool setSlotMinFilter(const base::Identifier* const);
};

inline GLuint Texture::getTexture() const          { return texture; }
inline int Texture::getWrapS() const               { return wrapS; }
inline int Texture::getWrapT() const               { return wrapT; }
inline int Texture::getMagFilter() const           { return magFilter; }
inline int Texture::getMinFilter() const           { return minFilter; }
inline GLfloat Texture::getRedScale() const        { return redScale; }
inline GLfloat Texture::getRedBias() const         { return redBias; }
inline GLfloat Texture::getGreenScale() const      { return greenScale; }
inline GLfloat Texture::getGreenBias() const       { return greenBias; }
inline GLfloat Texture::getBlueScale() const       { return blueScale; }
inline GLfloat Texture::getBlueBias() const        { return blueBias; }

}
}

#endif
