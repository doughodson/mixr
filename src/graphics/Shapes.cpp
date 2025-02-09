
#include "mixr/graphics/Shapes.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/graphics/ColorGradient.hpp"
#include <GL/glu.h>

namespace mixr {
namespace graphics {

//==============================================================================
// Class: Circle
//==============================================================================
IMPLEMENT_SUBCLASS(Circle, "Circle")
EMPTY_DELETEDATA(Circle)

BEGIN_SLOTTABLE(Circle)
    "radius",    // Radius of circle
    "filled",    // True if circle is filled
    "slices",    // Number of slices in the circle
END_SLOTTABLE(Circle)

BEGIN_SLOT_MAP(Circle)
    ON_SLOT(1, setSlotRadius, base::Number)
    ON_SLOT(2, setSlotFilled, base::Boolean)
    ON_SLOT(3, setSlotSlices, base::Integer)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Circle)
    ON_EVENT_OBJ(UPDATE_VALUE, updateRadius, base::Number)
END_EVENT_HANDLER()

Circle::Circle()
{
    STANDARD_CONSTRUCTOR()
}

void Circle::copyData(const Circle& org, const bool)
{
   BaseClass::copyData(org);
   radius = org.radius;
   filled = org.filled;
   slices = org.slices;
}

// Update the circle's radius
bool Circle::updateRadius(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setRadius(x->asDouble());
    return ok;
}

// Draw function
void Circle::drawFunc()
{
   BEGIN_DLIST
   GLUquadricObj *qobj = gluNewQuadric();
   if (filled) {
      gluQuadricDrawStyle(qobj, GLU_FILL);
   }
   else {
      gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
   }
   gluDisk( qobj, 0.,  radius, slices, 1);
   gluDeleteQuadric(qobj);
   END_DLIST
}

// Set slot functions
bool Circle::setSlotRadius(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setRadius(x->asDouble());
    return ok;
}

bool Circle::setSlotFilled(const base::Boolean* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setFilled(x->asBool());
    return ok;
}

bool Circle::setSlotSlices(const base::Integer* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setSlices(x->asInt());
    return ok;
}

//==============================================================================
// Class: OcclusionCircle
//==============================================================================
IMPLEMENT_SUBCLASS(OcclusionCircle, "OcclusionCircle")
EMPTY_DELETEDATA(OcclusionCircle)

BEGIN_SLOTTABLE(OcclusionCircle)
    "outerRadius",  // Outer radius of our circle
END_SLOTTABLE(OcclusionCircle)

BEGIN_SLOT_MAP(OcclusionCircle)
    ON_SLOT(1, setSlotOuterRadius, base::Number)
END_SLOT_MAP()

OcclusionCircle::OcclusionCircle()
{
    STANDARD_CONSTRUCTOR()
    setFilled(true);
    setSlices(100);
}

void OcclusionCircle::copyData(const OcclusionCircle& org, const bool)
{
    BaseClass::copyData(org);
    outerRadius = org.outerRadius;
}

// Draw function
void OcclusionCircle::drawFunc()
{
   BEGIN_DLIST
   GLUquadricObj* qobj = gluNewQuadric();
   if (isFilled()) {
      gluQuadricDrawStyle(qobj, GLU_FILL);
   }
   else {
      gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
   }
   gluDisk( qobj, getRadius(),  outerRadius, getSlices(), 1);
   gluDeleteQuadric(qobj);
   END_DLIST
}

// Set slot functions
bool OcclusionCircle::setSlotOuterRadius(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setOuterRadius(x->asDouble());
    return ok;
}

//==============================================================================
// Class: Arc
//==============================================================================
IMPLEMENT_SUBCLASS(Arc, "Arc")
EMPTY_DELETEDATA(Arc)

BEGIN_SLOTTABLE(Arc)
    "startAngle",    // Start angle of arc
    "arcLength",     // Length of arc
    "connect",       // if true, we connect the lines (so the circle looks like a piece of pie,
                     // instead of an open ended arc)
END_SLOTTABLE(Arc)

BEGIN_SLOT_MAP(Arc)
    ON_SLOT(1, setSlotStartAngle, base::Number)
    ON_SLOT(2, setSlotArcLength, base::Number)
    ON_SLOT(3, setSlotIsConnected, base::Boolean)
END_SLOT_MAP()

Arc::Arc()
{
    STANDARD_CONSTRUCTOR()
}

void Arc::copyData(const Arc& org, const bool)
{
   BaseClass::copyData(org);
   startAngle = org.startAngle;
   arcLength = org.arcLength;
   connected = org.connected;
}

// Draw function
void Arc::drawFunc()
{
   BEGIN_DLIST
   GLUquadricObj* qobj = gluNewQuadric();
   if (isFilled()) {
      gluQuadricDrawStyle(qobj, GLU_FILL);
   }
   else {
      gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
   }
   if (connected) {
      gluPartialDisk(qobj, 0, getRadius(), getSlices(), 2, startAngle, arcLength);
   }
   else {
      gluPartialDisk(qobj, getRadius(), getRadius(), getSlices(), 2, startAngle, arcLength);
   }
   gluDeleteQuadric(qobj);
   END_DLIST
}

// Set slot functions
bool Arc::setSlotStartAngle(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setStartAngle(x->asDouble());
    return ok;
}

bool Arc::setSlotArcLength(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setArcLength(x->asDouble());
    return ok;
}

bool Arc::setSlotIsConnected(const base::Boolean* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setIsConnected(x->asBool());
    return ok;
}


//==============================================================================
// Class: OcclusionArc
//==============================================================================
IMPLEMENT_SUBCLASS(OcclusionArc, "OcclusionArc")
EMPTY_DELETEDATA(OcclusionArc)

BEGIN_SLOTTABLE(OcclusionArc)
    "outerRadius",  // Outer radius of our arc
END_SLOTTABLE(OcclusionArc)

BEGIN_SLOT_MAP(OcclusionArc)
    ON_SLOT(1, setSlotOuterRadius, base::Number)
END_SLOT_MAP()

OcclusionArc::OcclusionArc()
{
    STANDARD_CONSTRUCTOR()
    setFilled(true);
    setSlices(100);
}

void OcclusionArc::copyData(const OcclusionArc& org, const bool)
{
    BaseClass::copyData(org);
    outerRadius = org.outerRadius;
}

// Draw function
void OcclusionArc::drawFunc()
{
   BEGIN_DLIST
   GLUquadricObj* qobj = gluNewQuadric();
   if (isFilled()) {
      gluQuadricDrawStyle(qobj, GLU_FILL);
   }
   else {
      gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
   }

   gluPartialDisk(qobj, getRadius(), outerRadius, getSlices(), 2, getStartAngle(), getArcLength());

   gluDeleteQuadric(qobj);
   END_DLIST
}

// Set slot functions
bool OcclusionArc::setSlotOuterRadius(const base::Number* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setOuterRadius(x->asDouble());
    return ok;
}

//==============================================================================
// Class: Point
//==============================================================================
IMPLEMENT_SUBCLASS(Point, "Point")
EMPTY_SLOTTABLE(Point)
EMPTY_CONSTRUCTOR(Point)
EMPTY_COPYDATA(Point)
EMPTY_DELETEDATA(Point)

// Draw function
void Point::drawFunc()
{
   BEGIN_DLIST
   const unsigned int n = getNumberOfVertices();
   const base::Vec3d* v = getVertices();
   glBegin(GL_POINTS);
   for (unsigned int i = 0; i < n; i++) {
      lcVertex3v( v[i].ptr() );
   }
   glEnd();
   END_DLIST
}


//==============================================================================
// Class: LineLoop
//==============================================================================
IMPLEMENT_SUBCLASS(LineLoop, "LineLoop")
EMPTY_SLOTTABLE(LineLoop)
EMPTY_CONSTRUCTOR(LineLoop)
EMPTY_COPYDATA(LineLoop)
EMPTY_DELETEDATA(LineLoop)

// Draw function
void LineLoop::drawFunc()
{
   BEGIN_DLIST
   const unsigned int n = getNumberOfVertices();
   const base::Vec3d* v = getVertices();
   if (n >= 2) {
      glBegin(GL_LINE_LOOP);
      for (unsigned int i = 0; i < n; i++) {
         lcVertex3v( v[i].ptr() );
      }
      glEnd();
   }
   END_DLIST
}


//==============================================================================
// Class: Line
//==============================================================================
IMPLEMENT_SUBCLASS(Line, "Line")
EMPTY_DELETEDATA(Line)

BEGIN_SLOTTABLE(Line)
    "segment",    // True if line segments
END_SLOTTABLE(Line)

BEGIN_SLOT_MAP(Line)
    ON_SLOT(1, setSlotSegments, base::Boolean)
END_SLOT_MAP()

Line::Line()
{
   STANDARD_CONSTRUCTOR()
}

void Line::copyData(const Line& org, const bool)
{
   BaseClass::copyData(org);
   segment = org.segment;
}

// Draw function
void Line::drawFunc()
{
   BEGIN_DLIST
   const unsigned int n = getNumberOfVertices();
   const base::Vec3d* v = getVertices();
   if (n >= 2) {
      if (segment) {
         // Draw as line segments (pairs of vertices)
         glBegin(GL_LINES);
         for (unsigned int i = 0; i < n; i++) {
            lcVertex3v( v[i].ptr() );
         }
         glEnd();
      }
      else {
         // Draw one long line
         glBegin(GL_LINE_STRIP);
         for (unsigned int i = 0; i < n; i++) {
            lcVertex3v( v[i].ptr() );
         }
         glEnd();
      }
   }
   END_DLIST
}

// Set slot functions
bool Line::setSlotSegments(const base::Boolean* const x)
{
    bool ok = false;
    if (x != nullptr) ok = setSegments(x->asBool());
    return ok;
}

//==============================================================================
// Class: Quad
//==============================================================================
IMPLEMENT_SUBCLASS(Quad, "Quad")
EMPTY_DELETEDATA(Quad)

BEGIN_SLOTTABLE(Quad)
    "strip",    // True if we want a quad strip
END_SLOTTABLE(Quad)

BEGIN_SLOT_MAP(Quad)
    ON_SLOT(1, setSlotStrip, base::Boolean)
END_SLOT_MAP()

Quad::Quad()
{
    STANDARD_CONSTRUCTOR()
}

void Quad::copyData(const Quad& org, const bool)
{
    BaseClass::copyData(org);

    strip = org.strip;
}

bool Quad::setSlotStrip(const base::Boolean* const x)
{
    bool ok = false;
    if (x != nullptr) {
        ok = setStrip(x->asBool());
    }
    return ok;
}

void Quad::drawFunc()
{
    bool ok = false;

    // Draw with texture
    const unsigned int nv = getNumberOfVertices();
    if (nv > 3) {
        if (!strip) {
            int rem = nv % 4;
            if (rem != 0) std::cerr << "Quad::drawFunc() - Quad have to have multiple of 4 vertices, add or remove vertices!!" << std::endl;
            else {
                BEGIN_DLIST
                glBegin(GL_QUADS);
                ok = true;
            }
        }
        else {
            int rem = nv % 2;
            if (rem != 0) std::cerr << "Quad::drawFunc() - quad strips have to have multiple of 2 vertices, add or remove vertices!!" << std::endl;
            else {
                BEGIN_DLIST
                glBegin(GL_QUAD_STRIP);
                ok = true;
            }
        }

        if (ok) {
            // get our regular vertices here
            const base::Vec3d* v = getVertices();

            const unsigned int ntc = getNumberOfTextureCoords();
            // draw with texture
            if (ntc > 0 && hasTexture()) {
                const base::Vec2d* texCoord = getTextureCoord();
                unsigned int tc = 0; // texture count
                for (unsigned int i = 0; i < nv; i++) {
                    // add our textures coordinates
                    if (tc < ntc)  lcTexCoord2v(texCoord[tc++].ptr());
                    // now our vertices
                    lcVertex3v( v[i].ptr() );
                }

            }
            // draw without texture
            else {
                // get our color gradient and apply it (if we have one)
                const auto colGradient = dynamic_cast<ColorGradient*>(getColor());

                for (unsigned int i = 0; i < nv; i++) {
                    if (colGradient != nullptr) {
                        base::IColor* col = colGradient->getColorByIdx(i+1);
                        if (col != nullptr)
                            glColor4f(static_cast<GLfloat>(col->red()),
                                      static_cast<GLfloat>(col->green()),
                                      static_cast<GLfloat>(col->blue()),
                                      static_cast<GLfloat>(col->alpha()));
                    }
                    // now add our vertex
                    lcVertex3v( v[i].ptr() );
                }
            }
            glEnd();
            END_DLIST
        }
    }

    else std::cerr << "Quad::drawFunc() - Quad or QuadStrip needs at least 4 vertices!" << std::endl;
}

//==============================================================================
// Class: Triangle
//==============================================================================
IMPLEMENT_SUBCLASS(Triangle, "Triangle")
EMPTY_DELETEDATA(Triangle)

BEGIN_SLOTTABLE(Triangle)
    "fan",    // True if we want a triangle fan
END_SLOTTABLE(Triangle)

BEGIN_SLOT_MAP(Triangle)
    ON_SLOT(1, setSlotFan, base::Boolean)
END_SLOT_MAP()

Triangle::Triangle()
{
    STANDARD_CONSTRUCTOR()
}

void Triangle::copyData(const Triangle& org, const bool)
{
    BaseClass::copyData(org);

    fan = org.fan;
}

bool Triangle::setSlotFan(const base::Boolean* const x)
{
    bool ok {};
    if (x != nullptr) {
        ok = setFan(x->asBool());
    }
    return ok;
}

void Triangle::drawFunc()
{
    // get our color gradient and apply it (if we have one)
    unsigned int nv {getNumberOfVertices()};

    bool ok {};
    if (nv > 2) {
        if (!strip && !fan) {
            const int rem = nv % 3;
            if (rem != 0) std::cerr << "Triangle::drawFunc() - Triangles have to have multiple of 3 vertices, add or remove vertices!!" << std::endl;
            else {
                BEGIN_DLIST
                glBegin(GL_TRIANGLES);
                ok = true;
            }
        }
        else if (fan) {
            BEGIN_DLIST
            glBegin(GL_TRIANGLE_FAN);
            ok = true;
        }
        else if (strip){
            BEGIN_DLIST
            glBegin(GL_TRIANGLE_STRIP);
            ok = true;
        }

        if (ok) {
            // get our regular vertices here
            const base::Vec3d* v {getVertices()};

            const unsigned int ntc {getNumberOfTextureCoords()};
            // draw with texture
            if (ntc > 0 && hasTexture()) {
                const base::Vec2d* texCoord = getTextureCoord();
                unsigned int tc = 0; // texture count
                for (unsigned int i = 0; i < nv; i++) {
                    // add our textures coordinates
                    if (tc < ntc)  lcTexCoord2v(texCoord[tc++].ptr());
                    // now our vertices
                    lcVertex3v( v[i].ptr() );
                }

            }
            // draw without texture
            else {
                // get our color gradient and apply it (if we have one)
                const auto colGradient = dynamic_cast<ColorGradient*>(getColor());

                for (unsigned int i = 0; i < nv; i++) {
                    if (colGradient != nullptr) {
                        base::IColor* col = colGradient->getColorByIdx(i+1);
                        if (col != nullptr)
                            glColor4f(static_cast<GLfloat>(col->red()),
                                      static_cast<GLfloat>(col->green()),
                                      static_cast<GLfloat>(col->blue()),
                                      static_cast<GLfloat>(col->alpha()));
                    }
                    // now add our vertex
                    lcVertex3v( v[i].ptr() );
                }
            }
            glEnd();
            END_DLIST
        }
    }
    else std::cerr << "Triangle::drawFunc() - Triangle or Triangle needs at least 3 vertices!" << std::endl;
}

}
}
