
#ifndef __mixr_graphics_Shapes_HPP__
#define __mixr_graphics_Shapes_HPP__

#include "Graphic.hpp"

namespace mixr {
namespace base { class INumber; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: Circle
//
// Factory name: Circle
// Slots:
//  radius    <INumber>   ! Radius of the circle (default: 1 )
//  filled    <Boolean>   ! True if circle is filled (default: false)
//  slices    <Integer>   ! Number of slices in the circle (i.e. number of sides)
//                        ! (default: 16)
//------------------------------------------------------------------------------
class Circle : public Graphic
{
    DECLARE_SUBCLASS(Circle, Graphic)

public:
    Circle();

    void drawFunc() override;

    // sets radius and returns true if successful.
    virtual bool setRadius(const double x)   { radius = x; return true; }
    // sets filled and returns true if successful.
    virtual bool setFilled(const bool x)     { filled = x; return true; }
    // sets slices and returns true if successful.
    virtual bool setSlices(const int x)      { slices = x; return true; }

    double getRadius()       { return radius; }
    bool isFilled()          { return filled; }
    int getSlices()          { return slices; }

    bool event(const int event, base::IObject* const obj = nullptr) override;

protected:
    // event functions
    bool updateRadius(const base::INumber* const x);

private:
    double radius {1.0};
    bool filled {};
    int slices {16};

private:
    // slot table helper methods
    bool setSlotRadius(const base::INumber* const);
    bool setSlotFilled(const base::Boolean* const);
    bool setSlotSlices(const base::Integer* const);
};

//------------------------------------------------------------------------------
// Class: OcclusionCircle
//
// Factory name: OcclusionCircle
// Slots:
//  outerRadius   <INumber>   ! Outer radius of our circle (default: 1.1 )
//
// Draws a nice doughnut type circle, based on inner and outer radius
//------------------------------------------------------------------------------
class OcclusionCircle : public Circle
{
    DECLARE_SUBCLASS(OcclusionCircle, Circle)

public:
    OcclusionCircle();

    void drawFunc() override;

    virtual bool setOuterRadius(const double x)     { outerRadius = x; return true; }

    double getOuterRadius()                         { return outerRadius; }

private:
    double outerRadius {1.1};        // portion that is occluded (just a bit bigger than inner radius)

private:
    bool setSlotOuterRadius(const base::INumber* const);
};


//------------------------------------------------------------------------------
// Class: Arc
//
// Factory name: Arc
// Slots:
//  startAngle   <INumber>   ! Start angle of the arc in degrees (default: 0 )
//  arcLength    <INumber>   ! length of the arc in degrees (default: 90)
//  connect      <Boolean>   ! if true, we connect the lines (so the circle looks like a piece of pie,
//                           ! instead of an open ended arc) (default: false)
//
// Public member functions:
//
//  bool setStartAngle(INumber* ssaobj)
//      Sets the start angle to ssaobj and returns true if successful.
//
//  bool setArcLength(INumber* seaobj)
//      Sets the arc length te seaobj and returns true if successful.
//
//  bool setIsConnected(INumber* iscobj)
//      Sets isConnected (boolean) and returns true if successful.
//
//------------------------------------------------------------------------------
class Arc : public Circle
{
    DECLARE_SUBCLASS(Arc, Circle)
public:
    Arc();

    void drawFunc() override;

    virtual bool setStartAngle(const double x)  { startAngle = x; return true; }
    virtual bool setArcLength(const double x)   { arcLength = x; return true; }
    virtual bool setIsConnected(const bool x)   { connected = x; return true; }

    double getStartAngle()                      { return startAngle; }
    double getArcLength()                       { return arcLength; }
    bool  isConnected()                         { return connected; }

private:
    double startAngle {};
    double arcLength {90.0};
    bool connected {};

private:
    // slot table helper methods
    bool setSlotStartAngle(const base::INumber* const);
    bool setSlotArcLength(const base::INumber* const);
    bool setSlotIsConnected(const base::Boolean* const);
};

//------------------------------------------------------------------------------
// Class: OcclusionArc
//
// Factory name: OcclusionArc
// Slots:
//  outerRadius   <Number>    ! Outer radius of our circle (default: 1.1)
//
// Just like OcclusionCircle, only draws from a start angle to a finish angle
//------------------------------------------------------------------------------
class OcclusionArc : public Arc
{
    DECLARE_SUBCLASS(OcclusionArc, Arc)

public:
    OcclusionArc();

    void drawFunc() override;

    bool setOuterRadius(const double x)  { outerRadius = x; return true; }

    double getOuterRadius()              { return outerRadius; }

private:
    // outer radius just a bit bigger than inner radius
    double outerRadius {1.1};          // portion that is occluded

private:
    // slot table helper methods
    bool setSlotOuterRadius(const base::INumber* const);
};


//------------------------------------------------------------------------------
// Class: Point
//
// Factory name: Point
//
// Public member functions:
//   void drawFunc() override
//   Draws a point for each vertex
//
//------------------------------------------------------------------------------
class Point : public Graphic
{
    DECLARE_SUBCLASS(Point, Graphic)
public:
    Point();
    void drawFunc() override;
};


//------------------------------------------------------------------------------
// Class: LineLoop
//
// Factory name: LineLoop
//
// Public member functions:
//    void drawFunc() override
//    Draws a line based on a set of vertices
//
//------------------------------------------------------------------------------
class LineLoop : public Graphic
{
    DECLARE_SUBCLASS(LineLoop, Graphic)
public:
    LineLoop();
    void drawFunc() override;
};

//------------------------------------------------------------------------------
// Class: Line
//
// Factory name: Line
// Slots:
//  segment   <Boolean>    ! True if line segments (default: false)
//
// Public member functions:
//
//  bool setSegments(Number* ssobj)
//      Sets segments (boolean) and returns true if successful.
//  bool isSegmented()
//      Returns number of segments.
//
//------------------------------------------------------------------------------
class Line : public Graphic
{
    DECLARE_SUBCLASS(Line, Graphic)
public:
    Line();

    void drawFunc() override;

    bool setSegments(const bool x)       { segment = x; return true; }

    bool isSegmented()                   { return segment; }

private:
    bool segment {};         // True if line segments

private:
    // slot table helper methods
    bool setSlotSegments(const base::Boolean* const);
};

//------------------------------------------------------------------------------
// Class: Quad
//
// Factory name: Quad
// Description:
//      Simple openGL quad strip
//------------------------------------------------------------------------------
class Quad : public Graphic
{
    DECLARE_SUBCLASS(Quad, Graphic)
public:
    Quad();

    bool setStrip(const bool x)     { strip = x; return true; }
    bool isStrip()                  { return strip; }

    void drawFunc() override;

protected:
    bool strip {};     // are we a Quad Strip?

private:
    // slot table helper methods
    bool setSlotStrip(const base::Boolean* const);
};

//------------------------------------------------------------------------------
// Class: Triangle
//
// Factory name: Triangle
// Description:
//      Simple openGL triangle primitive (or strip, or fan)
//------------------------------------------------------------------------------
class Triangle : public Quad
{
    DECLARE_SUBCLASS(Triangle, Quad)

public:
    Triangle();

    bool setFan(const bool x)     { fan = x; return true; }

    bool isFan()                  { return fan; }

    void drawFunc() override;

private:
    bool fan {};       // are we a triangle fan?

private:
    // slot table helper methods
    bool setSlotFan(const base::Boolean* const);
};

}
}

#endif
