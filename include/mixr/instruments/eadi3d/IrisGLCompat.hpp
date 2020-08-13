
#ifndef __mixr_instruments_IrisGLCompat_HPP__
#define __mixr_instruments_IrisGLCompat_HPP__

namespace mixr {
namespace instruments {

//------------------------------------------------------------------------------
// Class: IrisGLCompat
// Description: IrisGL compatibility
//------------------------------------------------------------------------------
class IrisGLCompat
{
public:
    IrisGLCompat() =default;

    // functions for drawing
    void arc(const float x, const float y, const float r, const float startAng, const float endAng);
    void arcf(const float x, const float y, const float r, const float startAng, const float endAng);
    void circ(const float x, const float y, const float r);
    void circf(const float x, const float y, const float r);
    void washerf(const float x, const float y, const float r1, const float r2);
    void rect(const float x1, const float y1, const float x2, const float y2);

    void rpmv2(const float x, const float y);
    void rpdr2(const float x, const float y);
    void rmv2(const float x, const float y);
    void rdr2(const float x, const float y);

private:
    float vertexX {};
    float vertexY {};
};

}
}

#endif
