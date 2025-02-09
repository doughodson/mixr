
#include "mixr/graphics/Polygon.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/graphics/ColorGradient.hpp"
#include "mixr/base/PairStream.hpp"
#include <GL/glu.h>

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Polygon, "Polygon")
EMPTY_SLOTTABLE(Polygon)
EMPTY_DELETEDATA(Polygon)

Polygon::Polygon()
{
   STANDARD_CONSTRUCTOR()
   coeff.set(0,0,0,0);
   norm.set(1,0,0);
}

void Polygon::copyData(const Polygon& org, const bool)
{
   BaseClass::copyData(org);

   coeff = org.coeff;
   coeffValid = org.coeffValid;
   norm = org.norm;
   layerValue = org.layerValue;
}

//------------------------------------------------------------------------------
// compute the vector normal to polygon
//------------------------------------------------------------------------------
bool Polygon::calcNormal()
{
   bool ok = false;
   if (getNumberOfVertices() >= 3) {
      ok = calcNormal(norm,getVertices());
   }
   else {
      norm.set(1.0f,0.0f,0.0f);
   }

   return ok;
}

bool Polygon::calcNormal(base::Vec3d& n, const base::Vec3d v[3])
{
   base::Vec3d t1 = v[1] - v[0];
   t1.normalize();

   base::Vec3d t2 = v[2] - v[1];
   t2.normalize();

   n = (t1 ^ t2); // cross product

   return true;
}

//------------------------------------------------------------------------------
// find the coefficients of the plane equation
//------------------------------------------------------------------------------
bool Polygon::calcPlaneCoeff(base::Vec4d& cc, const base::Vec3d v[3])
{
   cc[A] =  ( v[0].y() * (v[1].z() - v[2].z())
            - v[1].y() * (v[0].z() - v[2].z())
            + v[2].y() * (v[0].z() - v[1].z()) );

   cc[B] = -( v[0].x() * (v[1].z() - v[2].z())
            - v[1].x() * (v[0].z() - v[2].z())
            + v[2].x() * (v[0].z() - v[1].z()) );

   cc[C] =  ( v[0].x() * (v[1].y() - v[2].y())
            - v[1].x() * (v[0].y() - v[2].y())
            + v[2].x() * (v[0].y() - v[1].y()) );

   cc[D] = -( v[0].x() * (v[1].y()*v[2].z() - v[2].y()*v[1].z())
            - v[1].x() * (v[0].y()*v[2].z() - v[2].y()*v[0].z())
            + v[2].x() * (v[0].y()*v[1].z() - v[1].y()*v[0].z()) );

   return true;
}

bool Polygon::calcPlaneCoeff()
{
   if (getNumberOfVertices() >= 3) {
      coeffValid = calcPlaneCoeff(coeff,getVertices());
   }
   else {
      coeffValid = false;
      coeff.set(0,0,0,0);
   }

   return coeffValid;
}

//------------------------------------------------------------------------------
// compute the z value at point p using plane coefficients c.
//------------------------------------------------------------------------------
double Polygon::calcZ(const base::Vec2d& p, const base::Vec4d& cc)
{
   double zz = 0.0;
   zz = (-cc._v[D] - (cc._v[A] * p.x()) - (cc._v[B] * p.y()) )/cc._v[C];
   return zz;
}

double Polygon::calcZ(const base::Vec2d& p) const
{
   return calcZ(p,*getPlaneCoeff());
}

//------------------------------------------------------------------------------
// drawFunc()
//------------------------------------------------------------------------------
void Polygon::drawFunc()
{
    BEGIN_DLIST
    unsigned int nv = getNumberOfVertices();
    const base::Vec3d* vertices = getVertices();

    if (nv >= 2) {

        // Draw with texture
        unsigned int ntc = getNumberOfTextureCoords();
        if (ntc > 0 && hasTexture()) {
            const base::Vec2d* texCoord = getTextureCoord();
            unsigned int tc = 0; // texture count
            glBegin(GL_POLYGON);
            for (unsigned int i = 0; i < nv; i++) {
                if (tc < ntc)  {
                    lcTexCoord2v(texCoord[tc++].ptr());
                }
                lcVertex3v( vertices[i].ptr() );
            }
            glEnd();

        }

        // Draw without texture
        else {
            // get our color gradient, because if we have one, instead of a regular color, we will
            // override it here and set it on a per vertex level.
            const auto colGradient = dynamic_cast<ColorGradient*>(getColor());
            glBegin(GL_POLYGON);
            base::Vec3d* ptr = nullptr;
            for (unsigned int i = 0; i < nv; i++) {
                if (colGradient != nullptr) {
                    base::IColor* col = colGradient->getColorByIdx(i+1);
                    if (col != nullptr)
                       glColor4f(static_cast<GLfloat>(col->red()), static_cast<GLfloat>(col->green()),
                                 static_cast<GLfloat>(col->blue()), static_cast<GLfloat>(col->alpha()));
                }
                // if we have a material name, we will set up like we have a material
                if (getMaterialName() != nullptr) {
                    //lcVertex3v( vertices[i].ptr() );
                    ptr = const_cast<base::Vec3d*>(reinterpret_cast<const base::Vec3d*>(vertices[i].ptr()));

                    if (ptr != nullptr) {
                        calcNormal();
                        lcNormal3(norm.x(), norm.y(), -(norm.z()));
                        lcVertex3(ptr->x(), ptr->y(), ptr->z());
                    }
                }
                else {
                    lcVertex3v(vertices[i].ptr());
                }
            }
            glEnd();
        }
    }
    END_DLIST
}

}
}

