//------------------------------------------------------------------------------
// Shapes3D - shapes in 3D (mostly GLU shapes)
//------------------------------------------------------------------------------
#include "mixr/ui/glut/Shapes3D.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>

namespace mixr {
namespace glut {

IMPLEMENT_SUBCLASS(Sphere, "Sphere")

IMPLEMENT_SUBCLASS(Cylinder, "Cylinder")

IMPLEMENT_SUBCLASS(Cone, "Cone")
EMPTY_SLOTTABLE(Cone)

IMPLEMENT_SUBCLASS(Cube, "Cube")

IMPLEMENT_SUBCLASS(Torus, "Torus")

IMPLEMENT_SUBCLASS(Dodecahedron, "Dodecahedron")
EMPTY_SLOTTABLE(Dodecahedron)

IMPLEMENT_SUBCLASS(Tetrahedron, "Tetrahedron")
EMPTY_SLOTTABLE(Tetrahedron)

IMPLEMENT_SUBCLASS(Icosahedron, "Icosahedron")
EMPTY_SLOTTABLE(Icosahedron)

IMPLEMENT_SUBCLASS(Octahedron, "Octahedron")
EMPTY_SLOTTABLE(Octahedron)

IMPLEMENT_SUBCLASS(Teapot, "Teapot")
EMPTY_SLOTTABLE(Teapot)

// Sphere --
BEGIN_SLOTTABLE(Sphere)
    "stacks",       // 1: Number of stacks on the circle
END_SLOTTABLE(Sphere)

// Cylinder --
BEGIN_SLOTTABLE(Cylinder)
    "topRadius",    // 1: Radius at the top of the cylinder
    "height",       // 2: Height of the cylinder
END_SLOTTABLE(Cylinder)

// Cube --
BEGIN_SLOTTABLE(Cube)
    "size",       // 1: Size you want the cube to be
END_SLOTTABLE(Cube)

// Torus --
BEGIN_SLOTTABLE(Torus)
    "outerRadius", // 1: Radius of the outer ring
END_SLOTTABLE(Torus)

//------------------------------------------------------------------------------
//  Map slot tables
//------------------------------------------------------------------------------
BEGIN_SLOT_MAP(Sphere)
    ON_SLOT(1, setSlotStacks, base::Integer)
END_SLOT_MAP()

BEGIN_SLOT_MAP(Cylinder)
    ON_SLOT(1, setSlotTopRadius, base::INumber)
    ON_SLOT(2, setSlotHeight,    base::INumber)
END_SLOT_MAP()

BEGIN_SLOT_MAP(Cube)
    ON_SLOT(1, setSlotSize, base::INumber)
END_SLOT_MAP()

BEGIN_SLOT_MAP(Torus)
    ON_SLOT(1, setSlotOuterRadius, base::INumber)
END_SLOT_MAP()

Sphere::Sphere()
{
    STANDARD_CONSTRUCTOR()
}

Cylinder::Cylinder()
{
    STANDARD_CONSTRUCTOR()
}

Cone::Cone()
{
    STANDARD_CONSTRUCTOR()
}

Cube::Cube()
{
    STANDARD_CONSTRUCTOR()
}

Torus::Torus()
{
    STANDARD_CONSTRUCTOR()
}

Dodecahedron::Dodecahedron()
{
    STANDARD_CONSTRUCTOR()
}

Tetrahedron::Tetrahedron()
{
    STANDARD_CONSTRUCTOR()
}

Icosahedron::Icosahedron()
{
    STANDARD_CONSTRUCTOR()
}

Octahedron::Octahedron()
{
    STANDARD_CONSTRUCTOR()
}

Teapot::Teapot()
{
    STANDARD_CONSTRUCTOR()
}

void Sphere::copyData(const Sphere& org, const bool)
{
    BaseClass::copyData(org);
    stacks = org.stacks;
}

void Cylinder::copyData(const Cylinder& org, const bool)
{
    BaseClass::copyData(org);
    topRadius = org.topRadius;
    height = org.height;
}

EMPTY_COPYDATA(Cone)

void Cube::copyData(const Cube& org, const bool)
{
    BaseClass::copyData(org);
    size = org.size;
}

void Torus::copyData(const Torus& org, const bool)
{
    BaseClass::copyData(org);
    oRadius = org.oRadius;
}

EMPTY_COPYDATA(Dodecahedron)

EMPTY_COPYDATA(Tetrahedron)

EMPTY_COPYDATA(Icosahedron)

EMPTY_COPYDATA(Octahedron)

EMPTY_COPYDATA(Teapot)

EMPTY_DELETEDATA(Sphere)
EMPTY_DELETEDATA(Cylinder)
EMPTY_DELETEDATA(Cone)
EMPTY_DELETEDATA(Cube)
EMPTY_DELETEDATA(Torus)
EMPTY_DELETEDATA(Dodecahedron)
EMPTY_DELETEDATA(Tetrahedron)
EMPTY_DELETEDATA(Icosahedron)
EMPTY_DELETEDATA(Octahedron)
EMPTY_DELETEDATA(Teapot)

void Sphere::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidSphere(getRadius(), getSlices(), stacks);
    else glutWireSphere(getRadius(), getSlices(), stacks);
    END_DLIST
}

void Cylinder::drawFunc()
{
    BEGIN_DLIST
    GLUquadricObj *qobj = gluNewQuadric();
    if (isFilled()) gluQuadricDrawStyle(qobj, GLU_FILL);
    else gluQuadricDrawStyle(qobj, GLU_SILHOUETTE);
    gluCylinder(qobj, getRadius(), topRadius, height, getSlices(), getStacks());
    gluDeleteQuadric(qobj);
    END_DLIST
}

void Cone::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidCone(getRadius(), getHeight(), getSlices(), getStacks());
    else glutWireCone(getRadius(), getHeight(), getSlices(), getStacks());
    END_DLIST
}

void Cube::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidCube(size);
    else glutWireCube(size);
    END_DLIST
}

void Torus::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidTorus(getRadius(), oRadius, getSlices(), getStacks());
    else glutWireTorus(getRadius(), oRadius, getSlices(), getStacks());
    END_DLIST
}

void Dodecahedron::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidDodecahedron();
    else glutWireDodecahedron();
    END_DLIST
}

void Tetrahedron::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidTetrahedron();
    else glutWireTetrahedron();
    END_DLIST
}

void Icosahedron::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidIcosahedron();
    else glutWireIcosahedron();
    END_DLIST
}

void Octahedron::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidOctahedron();
    else glutWireOctahedron();
    END_DLIST
}

void Teapot::drawFunc()
{
    BEGIN_DLIST
    if (isFilled()) glutSolidTeapot(getSize());
    else glutWireTeapot(getSize());
    END_DLIST
}


bool Sphere::setSlotStacks(const base::Integer* const x)
{
    bool ok {};
    if (x != nullptr) ok = setStacks(x->asInt());
    return ok;
}
//------------------------------------------------------------------------------
//  setSlotTopRadius() -- for Cylinder
//------------------------------------------------------------------------------
bool Cylinder::setSlotTopRadius(const base::INumber* const x)
{
    bool ok {};
    if (x != nullptr) ok = setTopRadius(x->asDouble());
    return ok;
}
//------------------------------------------------------------------------------
//  setSlotHeight() -- for Cylinder
//------------------------------------------------------------------------------
bool Cylinder::setSlotHeight(const base::INumber* const x)
{
    bool ok{};
    if (x != nullptr) ok = setHeight(x->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotSize() -- for Cube
//------------------------------------------------------------------------------
bool Cube::setSlotSize(const base::INumber* const x)
{
    bool ok{};
    if (x != nullptr) ok = setSize(x->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
//  setSlotOuterRadius() -- for Torus
//------------------------------------------------------------------------------
bool Torus::setSlotOuterRadius(const base::INumber* const x)
{
    bool ok{};
    if (x != nullptr) ok = setOuterRadius(x->asDouble());
    return ok;
}

}
}
