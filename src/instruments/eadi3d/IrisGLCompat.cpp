
#include "mixr/instruments/eadi3d/IrisGLCompat.hpp"

#include "mixr/base/util/constants.hpp"
#include "mixr/base/util/platform_api.hpp"
#include "mixr/base/qty/util/angle_utils.hpp"

#include <GL/gl.h>
#include <cmath>

#define MAX_POINTS 64

namespace mixr {
namespace instruments {

void IrisGLCompat::arc(const float x, const float y, const float r, const float startAng, const float endAng)
{
    auto ang = static_cast<float>(startAng * base::angle::D2RCC);
    const auto fraction = static_cast<float>((endAng - startAng) / 360.0);
    const auto numPoints = static_cast<int>((static_cast<float>(MAX_POINTS)) * fraction);
    const auto delta = static_cast<float>((2.0 * base::PI * fraction) / static_cast<float>(numPoints));
    glBegin(GL_LINE_STRIP);
        for (int i{}; i < numPoints+1; i++) {
            const float vy{std::sin(ang) * r + y};
            const float vx{std::cos(ang) * r + x};
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

void IrisGLCompat::arcf(const float x, const float y, const float r, const float startAng, const float endAng)
{
    auto ang = static_cast<float>(startAng * base::angle::D2RCC);
    const auto fraction = static_cast<float>((endAng - startAng) / 360.0);
    const auto numPoints = static_cast<int>((static_cast<float>(MAX_POINTS)) * fraction);
    const auto delta = static_cast<float>((2.0 * base::PI * fraction) / static_cast<float>(numPoints));
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i{}; i < numPoints+1; i++) {
            const float vy{std::sin(ang) * r + y};
            const float vx{std::cos(ang) * r + x};
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

void IrisGLCompat::circ(const float x, const float y, const float r)
{
    float ang{};
    const auto delta = static_cast<float>((2.0 * base::PI) / static_cast<float>(MAX_POINTS));
    glBegin(GL_LINE_LOOP);
        for (int i{}; i < MAX_POINTS; i++) {
            const float vy{std::sin(ang) * r + y};
            const float vx{std::cos(ang) * r + x};
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

void IrisGLCompat::circf(const float x, const float y, const float r)
{
    float ang{};
    const auto delta = static_cast<float>((2.0 * base::PI) / static_cast<float>(MAX_POINTS));
    glBegin(GL_POLYGON);
        for (int i{}; i < MAX_POINTS; i++) {
            const float vy{std::sin(ang) * r + y};
            const float vx{std::cos(ang) * r + x};
            glVertex2f(vx, vy);
            ang += delta;
        }
    glEnd();
}

void IrisGLCompat::washerf(const float x, const float y, const float r1, const float r2)
{
    float ang{};
    const auto delta = static_cast<float>((2.0 * base::PI) / static_cast<float>(MAX_POINTS));
    glBegin(GL_TRIANGLE_STRIP);
        for (int i{}; i < MAX_POINTS+1; i++) {
            const float s{std::sin(ang)};
            const float c{std::cos(ang)};
            const float vy1{s * r1 + y};
            const float vx1{c * r1 + x};
            const float vy2{s * r2 + y};
            const float vx2{c * r2 + x};
            glVertex2f(vx1, vy1);
            glVertex2f(vx2, vy2);
            ang += delta;
        }
    glEnd();
}

void IrisGLCompat::rect(const float x1, const float y1, const float x2, const float y2)
{
    glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
        glVertex2f(x1, y2);
    glEnd();
}

void IrisGLCompat::rpmv2(const float x, const float y)
{
    vertexX = x;
    vertexY = y;
    glVertex2f(vertexX, vertexY);
}

void IrisGLCompat::rpdr2(const float x, const float y)
{
    vertexX += x;
    vertexY += y;
    glVertex2f(vertexX, vertexY);
}

void IrisGLCompat::rmv2(const float x, const float y)
{
    vertexX = x;
    vertexY = y;
    glVertex2f(vertexX, vertexY);
}

void IrisGLCompat::rdr2(const float x, const float y)
{
    vertexX += x;
    vertexY += y;
    glVertex2f(vertexX, vertexY);
}

}
}
