
#ifndef __mixr_instruments_Eadi3DObjects_H__
#define __mixr_instruments_Eadi3DObjects_H__

#include "mixr/base/Object.hpp"
#include "mixr/instruments/eadi3d/IrisGLCompat.hpp"

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: Eadi3DObjects
//
// Description:  Eadi3D font
//
// public methods (member functions):
//      (All of the classes derived from Objects have the same public methods.)
//
//      static GLenum createEadi3DObjects(GLuint fontBase)
//          Produces the stroke font map.
//
//------------------------------------------------------------------------------
class Eadi3DObjects : public base::Object
{
    DECLARE_SUBCLASS(Eadi3DObjects, base::Object)

public:
    Eadi3DObjects(const float radius = 1.625);

    enum EadiObj {
        FONT_0 = 0,    // Desc: Character '0'
        FONT_1 = 1,    // Desc: Character '1'
        FONT_2 = 2,    // Desc: Character '2'
        FONT_3 = 3,    // Desc: Character '3'
        FONT_4 = 4,    // Desc: Character '4'
        FONT_5 = 5,    // Desc: Character '5'
        FONT_6 = 6,    // Desc: Character '6'
        FONT_7 = 7,    // Desc: Character '7'
        FONT_8 = 8,    // Desc: Character '8'
        FONT_9 = 9,    // Desc: Character '9'
        PUNC05 = 45,    // Desc: Character '-'
        PUNC06 = 46,    // Desc: Character '.'
        PUNC07 = 47,    // Desc: Character '/'
        FONT_A = 65,    // Desc: Character 'A'
        FONT_B = 66,    // Desc: Character 'B'
        FONT_C = 67,    // Desc: Character 'C'
        FONT_D = 68,    // Desc: Character 'D'
        FONT_E = 69,    // Desc: Character 'E'
        FONT_F = 70,    // Desc: Character 'F'
        FONT_G = 71,    // Desc: Character 'G'
        FONT_H = 72,    // Desc: Character 'H'
        FONT_I = 73,    // Desc: Character 'I'
        FONT_J = 74,    // Desc: Character 'J'
        FONT_K = 75,    // Desc: Character 'K'
        FONT_L = 76,    // Desc: Character 'L'
        FONT_M = 77,    // Desc: Character 'M'
        FONT_N = 78,    // Desc: Character 'N'
        FONT_O = 79,    // Desc: Character 'O'
        FONT_P = 80,    // Desc: Character 'P'
        FONT_Q = 81,    // Desc: Character 'Q'
        FONT_R = 82,    // Desc: Character 'R'
        FONT_S = 83,    // Desc: Character 'S'
        FONT_T = 84,    // Desc: Character 'T'
        FONT_U = 85,    // Desc: Character 'U'
        FONT_V = 86,    // Desc: Character 'V'
        FONT_W = 87,    // Desc: Character 'W'
        FONT_X = 88,    // Desc: Character 'X'
        FONT_Y = 89,    // Desc: Character 'Y'
        FONT_Z = 90,    // Desc: Character 'Z'
        UP_ARROW = 100,
        DOWN_ARROW,
        GROUND_POINTER,
        BALL_FRAME,
        AC_REF,
        BORDER,
        HEADING_SCALE,
        LEFT_CARET,
        UP_CARET,
        UP_TRIANGLE,
        GLIDESLOPE,
        LOCALIZER,
        TURN_RATE,
        SLIP_IND,
        SLIP_BALL,
        ALPHA_SYMB,
        GLOBE,
        GS_TEXT1,
        GS_TEXT2,
        LOC_TEXT1,
        LOC_TEXT2,
        GROUND_POINTER_2,
        AC_REF2,
        UP_ARROW_OUTLINE,
        DOWN_ARROW_OUTLINE,
        UP_ARROW_SKINNY,
        DOWN_ARROW_SKINNY,
        __MAX_EADI_OBJECTS__
    };

    void makeObjects();
    void drawObj(EadiObj);
    IrisGLCompat irisgl;

    // drawing functions
    void makeFTZRO(unsigned int);
    void makeFT1(unsigned int);
    void makeFT2(unsigned int);
    void makeFT3(unsigned int);
    void makeFT4(unsigned int);
    void makeFT5(unsigned int);
    void makeFT6(unsigned int);
    void makeFT7(unsigned int);
    void makeFT8(unsigned int);
    void makeFT9(unsigned int);
    void makeFTMIN(unsigned int);
    void makeFTPER(unsigned int);
    void makeFTSLH(unsigned int);
    void makeFTA(unsigned int);
    void makeFTB(unsigned int);
    void makeFTC(unsigned int);
    void makeFTD(unsigned int);
    void makeFTE(unsigned int);
    void makeFTF(unsigned int);
    void makeFTG(unsigned int);
    void makeFTH(unsigned int);
    void makeFTI(unsigned int);
    void makeFTJ(unsigned int);
    void makeFTK(unsigned int);
    void makeFTL(unsigned int);
    void makeFTM(unsigned int);
    void makeFTN(unsigned int);
    void makeFTO(unsigned int);
    void makeFTP(unsigned int);
    void makeFTQ(unsigned int);
    void makeFTR(unsigned int);
    void makeFTS(unsigned int);
    void makeFTT(unsigned int);
    void makeFTU(unsigned int);
    void makeFTV(unsigned int);
    void makeFTW(unsigned int);
    void makeFTX(unsigned int);
    void makeFTY(unsigned int);
    void makeFTZ(unsigned int);

private:
    void makeFont();
    void makeGlobe();
    void drawArrow(float bAngle, float s, float c);
    void drawArrow2(float bAngle, float s, float c);

    unsigned int base{};
    float radius {1.625};
    bool made {};
};

}
}

#endif
