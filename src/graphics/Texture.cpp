
#include "mixr/graphics/Texture.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/Identifier.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Texture, "Texture")
EMPTY_DELETEDATA(Texture)

BEGIN_SLOTTABLE(Texture)
    "width",        //  1) Size of the texture map
    "height",       //  2) Size of the texture map
    "redScale",     //  3) glPixelTransferf(GL_RED_SCALE)
    "redBias",      //  4) glPixelTransferf(GL_RED_BIAS)
    "greenScale",   //  5) glPixelTransferf(GL_GREEN_SCALE)
    "greenBias",    //  6) glPixelTransferf(GL_GREEN_BIAS)
    "blueScale",    //  7) glPixelTransferf(GL_BLUE_SCALE)
    "blueBias",     //  8) glPixelTransferf(GL_BLUE_BIAS)
    "wrapS",        //  9) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S)
    "wrapT",        // 10) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T)
    "magFilter",    // 11) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER)
    "minFilter",    // 12) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER)
END_SLOTTABLE(Texture)

BEGIN_SLOT_MAP(Texture)
    ON_SLOT(1,  setSlotWidth,      base::Integer)
    ON_SLOT(2,  setSlotHeight,     base::Integer)
    ON_SLOT(3,  setSlotRedScale,   base::INumber)
    ON_SLOT(4,  setSlotRedBias,    base::INumber)
    ON_SLOT(5,  setSlotGreenScale, base::INumber)
    ON_SLOT(6,  setSlotGreenBias,  base::INumber)
    ON_SLOT(7,  setSlotBlueScale,  base::INumber)
    ON_SLOT(8,  setSlotBlueBias,   base::INumber)
    ON_SLOT(9,  setSlotWrapS,      base::Identifier)
    ON_SLOT(10, setSlotWrapT,      base::Identifier)
    ON_SLOT(11, setSlotMagFilter,  base::Identifier)
    ON_SLOT(12, setSlotMinFilter,  base::Identifier)
END_SLOT_MAP()

Texture::Texture()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void Texture::initData()
{
    setWidth(256);
    setHeight(256);
    setFormat(GL_RGB);
    setNumComponents(3);
}

void Texture::copyData(const Texture& org, const bool cc)
{
    BaseClass::copyData(org);
    if (cc) initData();

    texture = 0;  // copies need their own texture identifier

    wrapS = org.wrapS;
    wrapT = org.wrapT;
    magFilter = org.magFilter;
    minFilter = org.minFilter;
    redScale = org.redScale;
    redBias = org.redBias;
    greenScale = org.greenScale;
    greenBias = org.greenBias;
    blueScale = org.blueScale;
    blueBias = org.blueBias;
}

//------------------------------------------------------------------------------
// loadTexture() - load our texture
//------------------------------------------------------------------------------
void Texture::loadTexture()
{
   if (getPixels() != nullptr) {
      // get our texture ID
        GLuint txt = getTexture();

      // or if needed, generate the texture ID
        if (txt == 0) {
            glGenTextures(1, &txt);
            setTexture(txt);
        }

      // Load the texture
      if (txt != 0) {
            glBindTexture(GL_TEXTURE_2D, txt);
            glPixelTransferf(GL_RED_SCALE,   getRedScale());
            glPixelTransferf(GL_RED_BIAS,    getRedBias());
            glPixelTransferf(GL_GREEN_SCALE, getGreenScale());
            glPixelTransferf(GL_GREEN_BIAS,  getGreenBias());
            glPixelTransferf(GL_BLUE_SCALE,  getBlueScale());
            glPixelTransferf(GL_BLUE_BIAS,   getBlueBias());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getWrapS());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getWrapT());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getMagFilter());
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMinFilter());
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, getNumComponents(), getWidth(), getHeight(), 0, getFormat(), GL_UNSIGNED_BYTE, getPixels());
        }
    }
    else {
        if (isMessageEnabled(MSG_WARNING)) {
            std::cerr << "Texture::loadTexture() - no pixels found to load!" << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
// Set functions ....
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Set texture id
//------------------------------------------------------------------------------
void Texture::setTexture(const GLuint t)
{
   texture = t;
}

//------------------------------------------------------------------------------
// setWidth() - sets the width (in pixels) of our texture
//------------------------------------------------------------------------------
bool Texture::setWidth(const unsigned int size)
{
    // Make sure we're a power of 2
    unsigned int i = size;
    if (i >= 2048)      i = 2048;
    else if (i >= 1024) i = 1024;
    else if (i >= 512)  i = 512;
    else if (i >= 256)  i = 256;
    else if (i >= 128)  i = 128;
    else if (i >= 64)   i =  64;
    else if (i >= 32)   i =  32;
    else                i = 16;

    // check for re-scaling
    if (size != i) {
       if (isMessageEnabled(MSG_WARNING)) {
           std::cerr << "Texture::setTextureWidth(): changing invalid texture size from " << size << " to " << i << std::endl;
       }
    }

    // Set the new size
    BaseClass::setWidth(i);

    return true;
}

//------------------------------------------------------------------------------
// setHeight() - sets the height (in pixels) of our texture
//------------------------------------------------------------------------------
bool Texture::setHeight(const unsigned int size)
{
    // Make sure we're a power of 2
    unsigned int i = size;
    if (i >= 2048)      i = 2048;
    else if (i >= 1024) i = 1024;
    else if (i >= 512)  i = 512;
    else if (i >= 256)  i = 256;
    else if (i >= 128)  i = 128;
    else if (i >= 64)   i =  64;
    else if (i >= 32)   i =  32;
    else                i = 16;

    // check for re-scaling
    if (size != i) {
       if (isMessageEnabled(MSG_WARNING)) {
           std::cerr << "Texture::setTextureWidth(): changing invalid texture size from " << size << " to " << i << std::endl;
       }
    }

    // Set the new size
    BaseClass::setHeight(i);

    return true;
}

//------------------------------------------------------------------------------
// setRedScale() -- sets the red scale
//------------------------------------------------------------------------------
bool Texture::setRedScale(const GLfloat s)
{
    redScale= s;
    return true;
}

//------------------------------------------------------------------------------
// setRedBias() -- sets the red bias
//------------------------------------------------------------------------------
bool Texture::setRedBias(const GLfloat b)
{
    redBias = b;
    return true;
}

//------------------------------------------------------------------------------
// setGreenScale() -- sets the green scale
//------------------------------------------------------------------------------
bool Texture::setGreenScale(const GLfloat s)
{
    greenScale = s;
    return true;
}

//------------------------------------------------------------------------------
// setGreenBias() -- sets the green bias
//------------------------------------------------------------------------------
bool Texture::setGreenBias(const GLfloat b)
{
    greenBias = b;
    return true;
}

//------------------------------------------------------------------------------
// setBlueScale() -- sets the blue scale
//------------------------------------------------------------------------------
bool Texture::setBlueScale(const GLfloat s)
{
    blueScale = s;
    return true;
}

//------------------------------------------------------------------------------
// setBlueBias() -- sets the blue bias
//------------------------------------------------------------------------------
bool Texture::setBlueBias(const GLfloat b)
{
    blueBias = b;
    return true;
}

//------------------------------------------------------------------------------
// setWrapS() -- sets the GL_TEXTURE_WRAP_S parameter
//------------------------------------------------------------------------------
bool Texture::setWrapS(const int v)
{
    bool ok = false;
    if (v == GL_CLAMP || v == GL_REPEAT) {
        wrapS = v;
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setWrapT() -- sets the GL_TEXTURE_WRAP_T parameter
//------------------------------------------------------------------------------
bool Texture::setWrapT(const int v)
{
    bool ok = false;
    if (v == GL_CLAMP || v == GL_REPEAT) {
        wrapT = v;
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setMagFilter() -- sets the GL_TEXTURE_MAG_FILTER  parameter
//------------------------------------------------------------------------------
bool Texture::setMagFilter(const int v)
{
    bool ok = false;
    if (v == GL_NEAREST || v == GL_LINEAR) {
        magFilter = v;
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setMinFilter() -- sets the GL_TEXTURE_MIN_FILTER parameter
//------------------------------------------------------------------------------
bool Texture::setMinFilter(const int v)
{
    bool ok = false;
    if (v == GL_NEAREST || v == GL_LINEAR) {
        minFilter = v;
        ok = true;
    }
    return ok;
}


//------------------------------------------------------------------------------
// Set slot routines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// setSlotWidth() - sets our width slot
//------------------------------------------------------------------------------
bool Texture::setSlotWidth(const base::Integer* const size)
{
    bool ok = false;
    if (size != nullptr) ok = setWidth(size->asInt());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotHeight() - sets our height slot
//------------------------------------------------------------------------------
bool Texture::setSlotHeight(const base::Integer* const size)
{
    bool ok = false;
    if (size != nullptr) ok = setHeight(size->asInt());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotRedScale() - sets the red scale
//------------------------------------------------------------------------------
bool Texture::setSlotRedScale(const base::INumber* const s)
{
    bool ok = false;
    if (s != nullptr) ok = setRedScale(static_cast<GLfloat>(s->asDouble()));
    return ok;
}

//------------------------------------------------------------------------------
// setSlotRedBias() - sets the red bias
//------------------------------------------------------------------------------
bool Texture::setSlotRedBias(const base::INumber* const b)
{
    bool ok = false;
    if (b != nullptr) ok = setRedBias(static_cast<GLfloat>(b->asDouble()));
    return ok;
}

//------------------------------------------------------------------------------
// setSlotGreenScale() - sets the blue scale
//------------------------------------------------------------------------------
bool Texture::setSlotGreenScale(const base::INumber* const s)
{
    bool ok = false;
    if (s != nullptr) ok = setGreenScale(static_cast<GLfloat>(s->asDouble()));
    return ok;
}

//------------------------------------------------------------------------------
// setSlotGreenBias() - sets the green bias
//------------------------------------------------------------------------------
bool Texture::setSlotGreenBias(const base::INumber* const b)
{
    bool ok = false;
    if (b != nullptr) ok = setGreenBias(static_cast<GLfloat>(b->asDouble()));
    return ok;
}

//------------------------------------------------------------------------------
// setSlotBlueScale() - sets the blue scale
//------------------------------------------------------------------------------
bool Texture::setSlotBlueScale(const base::INumber* const s)
{
    bool ok = false;
    if (s != nullptr) ok = setBlueScale(static_cast<GLfloat>(s->asDouble()));
    return ok;
}

//------------------------------------------------------------------------------
// setSlotBlueBias() - sets the blue bias
//------------------------------------------------------------------------------
bool Texture::setSlotBlueBias(const base::INumber* const b)
{
    bool ok = false;
    if (b != nullptr) ok = setBlueBias(static_cast<GLfloat>(b->asDouble()));
    return ok;
}

//------------------------------------------------------------------------------
// setSlotWrapS() -- sets the wrapS slot
//------------------------------------------------------------------------------
bool Texture::setSlotWrapS(const base::Identifier* const v)
{
    bool ok = false;
    if (v != nullptr) {
        if (*v == "clamp") ok = setWrapS(GL_CLAMP);
        else if (*v == "repeat") ok = setWrapS(GL_REPEAT);
        else {
              if (isMessageEnabled(MSG_WARNING)) {
                  std::cerr << "Texture::setSlotWrapS() invalid parameter: must be clamp or repeat." << std::endl;
              }
         }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotWrapT() -- sets the wrapT slot
//------------------------------------------------------------------------------
bool Texture::setSlotWrapT(const base::Identifier* const v)
{
    bool ok = false;
    if (v != nullptr) {
        if (*v == "clamp") ok = setWrapT(GL_CLAMP);
        else if (*v == "repeat") ok = setWrapT(GL_REPEAT);
        else {
              if (isMessageEnabled(MSG_WARNING)) {
                  std::cerr << "Texture::setSlotWrapT() invalid parameter: must be clamp or repeat." << std::endl;
              }
         }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMagFilter() -- sets the magFilter slot
//------------------------------------------------------------------------------
bool Texture::setSlotMagFilter(const base::Identifier* const v)
{
    bool ok = false;
    if (v != nullptr) {
        if (*v == "nearest") ok = setMagFilter(GL_NEAREST);
        else if (*v == "linear") ok = setMagFilter(GL_LINEAR);
        else {
              if (isMessageEnabled(MSG_WARNING)) {
                  std::cerr << "Texture::setSlotMagFilter() invalid parameter: must be nearest or linear." << std::endl;
              }
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotMinFilter() -- sets the minFilter slot
//------------------------------------------------------------------------------
bool Texture::setSlotMinFilter(const base::Identifier* const v)
{
    bool ok = false;
    if (v != nullptr) {
        if (*v == "nearest") ok = setMinFilter(GL_NEAREST);
        else if (*v == "linear") ok = setMinFilter(GL_LINEAR);
        else {
              if (isMessageEnabled(MSG_WARNING)) {
                  std::cerr << "Texture::setSlotMinFilter() invalid parameter: must be nearest or linear." << std::endl;
              }
        }
    }
    return ok;
}

}
}
