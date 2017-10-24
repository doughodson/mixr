
#ifndef __mixr_graphics_AbstractFtglFont_H__
#define __mixr_graphics_AbstractFtglFont_H__

#include "AbstractFont.hpp"

namespace mixr {
namespace base { class Number; }
namespace graphics {

//----------------------------------------------------------------------------------------------------------------------------------
// Class: AbstractFtglFont
//
// Description: Abstract class for all Freetype2 font classes. This hierarchy of types
//              depends upon freetype 2 and OpenGL libraries.
//
// Factory name: AbstractFTGLFont
//
// Slots:
//  faceSize      <Number>    ! font face size (default: 1)
//
// public methods (member functions):
//
//  unsigned int getFaceSize()
//  -- Gets the face size of the font.
//
//  private slot methods:
//
//  setSlotFaceSize(Number* faceSize) - sets the face size in points (1/72 of an inch per point)
//      - this is different than setting character width and height, as it
//      - expands and retracts the font proportionally.
//
// All font files NEED the following info to work correctly:
//      - path to the font  "path"  ie.. "C:WINNT\FONTS"
//      - font name         "file"  ie.. "arial.ttf"
// all other slots are optional
//
// The fontWidth and fontHeight will SCALE the individual width and height, however
// faceSize will SET the actual font width and height symmetrically (ie.. X by X instead
// of X by Y).  You can both set the fontWidth and Height and faceSize, and that
// will scale and then draw a font of certain facesize.
//
//---------------------------------------------------------------------------------
class AbstractFtglFont : public AbstractFont
{
    DECLARE_SUBCLASS(AbstractFtglFont, AbstractFont)

public:
    AbstractFtglFont();

    // Get Functions
    unsigned int getFaceSize()      { return fSize; };

private:
    static const int DEFAULT_FACE_SIZE;
    int fSize {DEFAULT_FACE_SIZE};      // face size

private:
    // slot table help methods
    bool setSlotFaceSize(const base::Number* const);
};

}
}

#endif
