//------------------------------------------------------------------------------
// Description:  3-dimensional shapes that can be drawn.  NOTE - these shapes
// all use GLUT or GLU, so you have to call the factory to serialize them
// from the input, and you also have to link to glut or freeglut to draw them.
//------------------------------------------------------------------------------
#ifndef __mixr_ui_glut_Shapes3D_HPP__
#define __mixr_ui_glut_Shapes3D_HPP__

#include "mixr/graphics/Shapes.hpp"

namespace mixr {
namespace base { class Integer; class INumber; }
namespace glut {

//------------------------------------------------------------------------------
// Class: Sphere
// Description - draws a sphere with X slices and Y stacks
//
// Factory name: Sphere
// Slots:
//    stacks   <INumber>    ! Number of stacks on the circle (default: 10)
//
// Note - all sphere and derived classes are automatically normalized at
// rendering (GLU does it)
//------------------------------------------------------------------------------
class Sphere : public graphics::Circle
{
    DECLARE_SUBCLASS(Sphere, graphics::Circle)

public:
    Sphere();

    void drawFunc() override;

    virtual bool setStacks(const int x)       { stacks = x; return true; }
    int getStacks()                           { return stacks; }

protected:
    bool setSlotStacks(const base::Integer* const);

private:
    int stacks {10};
};

//------------------------------------------------------------------------------
// Class: Cylinder
// Description - adds a top radius and height and draws a cylinder
//
// Factory name: Cylinder
// Slots:
//    topRadius  <INumber>  ! Radius at the top of the cylinder (default: 1)
//    height     <INumber>  ! Height of the cylinder (default: 1)
//
//------------------------------------------------------------------------------
class Cylinder : public Sphere
{
    DECLARE_SUBCLASS(Cylinder, Sphere)
public:
    Cylinder();
    void drawFunc() override;

    virtual bool setTopRadius(const double x)   { topRadius = x; return true; }
    virtual bool setHeight(const double x)      { height = x; return true; }
    double getTopRadius()                       { return topRadius; }
    double getHeight()                          { return height; }

private:
    double topRadius {1.0};
    double height {1.0};

private:
   // slot table helper methods
   bool setSlotTopRadius(const base::INumber* const);
   bool setSlotHeight(const base::INumber* const);
};

//------------------------------------------------------------------------------
// Class: Circle -> Sphere -> Cylinder -> Cone
// Description - uses everything but top radius to draw a cone
//
// Factory name: Cone
//
//------------------------------------------------------------------------------
class Cone : public Cylinder
{
    DECLARE_SUBCLASS(Cone, Cylinder)
public:
    Cone();
    void drawFunc() override;
};

//------------------------------------------------------------------------------
// Class: Cube
// Description - Draws a simple cube.  I derived it from Circle to use the filled
// flag only.  All other flags are ignored.
//
// Factory name: Cube
// Slots:
//    size    <INumber>    ! Size you want the cube to be (default: 1)
//
//------------------------------------------------------------------------------
class Cube : public graphics::Circle
{
    DECLARE_SUBCLASS(Cube, graphics::Circle)
public:
    Cube();
    void drawFunc() override;

    virtual bool setSize(const double x)   { size = x; return true; }
    double getSize()                       { return size; }

private:
    double size {1.0};

private:
   // slot table helper methods
   bool setSlotSize(const base::INumber* const);
};

//------------------------------------------------------------------------------
// Class: Torus
// Description - Uses filled slot to determine if wireframe or not.  Uses slices
// as number of sides, and stacks as number of rings
//
// Factory name: Torus
// Slots:
//   outerRadius    <INumber>          ! Radius of the outer ring (default: 2)
//
//------------------------------------------------------------------------------
class Torus : public Sphere
{
    DECLARE_SUBCLASS(Torus, Sphere)
public:
    Torus();
    void drawFunc() override;

    virtual bool setOuterRadius(const double x)    { oRadius = x; return true; }
    double getOuterRadius()                        { return oRadius; }

private:
    double oRadius {2.0};

private:
   // slot table helper methods
   bool setSlotOuterRadius(const base::INumber* const);
};


//------------------------------------------------------------------------------
// Class: Circle -> Dodecahedron
// Description - Draws a 12 faced figure which can be scaled
//
// Factory name: Dodecahedron
//
//------------------------------------------------------------------------------
class Dodecahedron : public graphics::Circle
{
    DECLARE_SUBCLASS(Dodecahedron, graphics::Circle)
public:
    Dodecahedron();
    void drawFunc() override;
};

//------------------------------------------------------------------------------
// Class: Circle -> Tetrahedron
// Description - Draws a 4 faced figure which can be scaled
//
// Factory name: Tetrahedron
//
//------------------------------------------------------------------------------
class Tetrahedron : public graphics::Circle
{
    DECLARE_SUBCLASS(Tetrahedron, graphics::Circle)
public:
    Tetrahedron();
    void drawFunc() override;
};

//------------------------------------------------------------------------------
// Class: Circle -> Icosahedron
// Description - Draws a 20 faced figure which can be scaled
//
// Factory name: Icosahedron
//
//------------------------------------------------------------------------------
class Icosahedron : public graphics::Circle
{
    DECLARE_SUBCLASS(Icosahedron, graphics::Circle)
public:
    Icosahedron();
    void drawFunc() override;
};

//------------------------------------------------------------------------------
// Class: Circle -> Octahedron
// Description - Draws a 8 faced figure which can be scaled
//
// Factory name: Octahedron
//
//------------------------------------------------------------------------------
class Octahedron : public graphics::Circle
{
    DECLARE_SUBCLASS(Octahedron, graphics::Circle)
public:
    Octahedron();
    void drawFunc() override;
};

//------------------------------------------------------------------------------
// Class: Cube -> Teapot
// Description - Draws the Utah Teapot, with a given size
//
// Factory name: Teapot
//
//------------------------------------------------------------------------------
class Teapot : public Cube
{
    DECLARE_SUBCLASS(Teapot, Cube)
public:
    Teapot();
    void drawFunc() override;
};

}
}

#endif
