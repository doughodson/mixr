
#ifndef __mixr_base_Transform_H__
#define __mixr_base_Transform_H__

#include "mixr/base/Object.hpp"
#include "mixr/base/osg/Matrixd"

#include <array>

namespace mixr {
namespace base {
class Angle;
class Number;

//------------------------------------------------------------------------------
// Class: Transform
// Factory name:Transform
// Slots:
//  x    <Number>   ! x transform (default: identity)
//  x    <Angle>    ! x transform (default: identity)
//  y    <Number>   ! y transform (default: identity)
//  y    <Angle>    ! y transform (default: identity)
//  z    <Number>   ! z transform (default: identity)
//  z    <Angle>    ! z transform (default: identity)
//  w    <Number>   ! w transform (default: identity)
//  w    <Angle>    ! w transform (default: identity)
//
// Description: Coordinate Transformations (e.g., translate, rotate, scale)
//
//
// Public methods:
//
//      operator const Matrixd&()
//          Returns a copy of the transformation matrix
//
// Private slot methods:
//      bool setSlotComputeMatrix1(Angle* sc1obj)
//      bool setSlotComputeMatrix1(Number* sc1obj)
//      ...
//      bool setSlotComputeMatrix4(Number* sc4obj)
//          Set the n'th parameter to the transformation function
//          and create the transformation matrix.  Returns true if the matrix
//          is valid.  These functions must be called in order.  For example,
//          "x" must be set before "y".
//
//------------------------------------------------------------------------------
class Transform : public Object
{
   DECLARE_SUBCLASS(Transform, Object)

public:
   Transform();
   operator const Matrixd&()                                         { return m; }

protected:
   virtual void computeMatrix();
   Matrixd m;                   // transformation matrix
   std::array<double, 4> v {};  // values
   int nv {};                   // Number of values

private:
   // slot table helper methods
   bool setSlotComputematrix1(const Angle* const);
   bool setSlotComputematrix1(const Number* const);
   bool setSlotComputematrix2(const Angle* const);
   bool setSlotComputematrix2(const Number* const);
   bool setSlotComputematrix3(const Angle* const);
   bool setSlotComputematrix3(const Number* const);
   bool setSlotComputematrix4(const Angle* const);
   bool setSlotComputematrix4(const Number* const);
};

//------------------------------------------------------------------------------
// Class: Translation
//
// Description: Coordinate Transformations (e.g., translate)
//
// Translation:
//    Two Parameters: Translate X by a factor of "x" and Y by a factor of "y".
//    Three Parameters: Translate X, Y and Z by factors of "x", "y" and "z" respectively.
//
// Factory names: Translation :
//    1. x  (Number)  x translation.
//    2. y  (Number)  y translation.
//    3. z  (Number)  z translation (optional).
//
//
//------------------------------------------------------------------------------
class Translation : public Transform
{
   DECLARE_SUBCLASS(Translation, Transform)
public:
   Translation();
protected:
   void computeMatrix() override;
};

//------------------------------------------------------------------------------
// Class: Rotation
//
// Description: Coordinate Transformations (e.g., rotate)
//
// Rotation:
//    One parameter: Rotate "w" radians about the Z axis.
//    Four parameters: Rotate "w" radians about the vector (x,y,z).
// Slots:
//    1. x  (Number)  x component of vector (optional)
//    2. y  (Number)  y component of vector (optional)
//    3. z  (Number)  z component of vector (optional)
//    4. w  (Angle)   radians to rotate about vector
//
//
//------------------------------------------------------------------------------
class Rotation : public Transform
{
   DECLARE_SUBCLASS(Rotation, Transform)
public:
   Rotation();
protected:
   void computeMatrix() override;
};

//------------------------------------------------------------------------------
// Class: Scale
//
// Description: Coordinate Transformations (e.g., scale)
//
// Scale:
//    One parameter: Scale both X and Y by a factor of "x".
//    Two parameters: Scale X by a factor of "x" and Y by a factor of "y".
//    Three Parameters: Scale X, Y and Z by factors of "x", "y" and "z" respectively.
// Slots:
//    1. x  (Number)  x scaling factor
//    2. y  (Number)  y scaling factor (optional)
//    3. z  (Number)  z scaling factor (optional)
//
//
//------------------------------------------------------------------------------
class Scale : public Transform
{
   DECLARE_SUBCLASS(Scale, Transform)
public:
   Scale();
protected:
   void computeMatrix() override;
};

}
}

#endif
