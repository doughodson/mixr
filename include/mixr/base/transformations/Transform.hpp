
#ifndef __mixr_base_Transform_HPP__
#define __mixr_base_Transform_HPP__

#include "mixr/base/Object.hpp"
#include "mixr/base/osg/Matrixd"

#include <array>

namespace mixr {
namespace base {
class IAngle;
class INumber;

//------------------------------------------------------------------------------
// Class: Transform
// Description: Abstract class for coordinate transformations (e.g., translate, rotate, scale)
//------------------------------------------------------------------------------
// Factory name: Transform
//------------------------------------------------------------------------------
// Slots:
//  x    <Number>   ! x transform (default: identity)
//  x    <Angle>    ! x transform (default: identity)
//  y    <Number>   ! y transform (default: identity)
//  y    <Angle>    ! y transform (default: identity)
//  z    <Number>   ! z transform (default: identity)
//  z    <Angle>    ! z transform (default: identity)
//  w    <Number>   ! w transform (default: identity)
//  w    <Angle>    ! w transform (default: identity)
//------------------------------------------------------------------------------
class Transform : public Object
{
   DECLARE_SUBCLASS(Transform, Object)

public:
   Transform();
   // returns a copy of the transformation matrix
   explicit operator const Matrixd&()              { return m; }

protected:
   Matrixd m;                   // transformation matrix
   std::array<double, 4> v{};   // values
   int nv{};                    // Number of values

private:
   virtual void computeMatrix() =0;

private:
   // slot table helper methods
   // sets the n'th parameter to the transformation function
   // and create the transformation matrix.  Returns true if the matrix
   // is valid.  These functions must be called in order.  For example,
   // "x" must be set before "y".
   bool setSlotComputematrix1(const IAngle* const);
   bool setSlotComputematrix1(const INumber* const);

   bool setSlotComputematrix2(const IAngle* const);
   bool setSlotComputematrix2(const INumber* const);

   bool setSlotComputematrix3(const IAngle* const);
   bool setSlotComputematrix3(const INumber* const);

   bool setSlotComputematrix4(const IAngle* const);
   bool setSlotComputematrix4(const INumber* const);
};

}
}

#endif
